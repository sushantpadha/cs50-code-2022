from copy import copy
import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Commented for check50 (`render_markdown()` and `info()` depend on this)
# Can't import extra python modules :(

from markdown import Markdown  # import markdown support

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.template_global()
def static_include(filename):
    """Allow `include` from the static folder"""
    # https://stackoverflow.com/a/44777196/15083607
    fullpath = os.path.join(app.static_folder, filename)
    with open(fullpath, 'r') as f:
        return f.read()


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    COLUMNS = {
        "Symbol": 0,
        "Name": 0,
        "Shares": 2,
        "Price": 2,
        "% Change": 0,
        "TOTAL": 2
    }
    COLUMN_NAMES, ALIGNMENT = list(COLUMNS.keys()), list(COLUMNS.values())
    USD_COLUMNS = [3,5]

    try:
        # Net assets of user
        assets, total, cash = get_assets(session["user_id"])
    except RuntimeError:
        return apology("could not retrieve user assets", 500)

    # Create data
    data = {
        "columns": COLUMN_NAMES,
        "alignment": ALIGNMENT,
        "assets": list(assets.values()),
        "usd_columns": USD_COLUMNS,
        "cash": cash,
        "total": total
    }

    return render_template("index.html", **data)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        symbol = request.form.get("symbol")
        shares = request.form.get("shares")

        if not symbol:
            return apology("missing symbol", 400)
        if not shares:
            return apology("missing shares", 400)
        try:
            shares = int(shares)
            if shares < 1:
                raise ValueError
        except (ValueError, TypeError):
            return apology("invalid shares value", 400)

        # Lookup symbol
        quotation = lookup(symbol)

        if quotation is None:
            return apology("invalid symbol", 400)

        # * see ./db_info.md

        price = quotation["price"]

        total = shares * price

        try:
            # Get user cash
            rows = db.execute(
                "SELECT cash FROM users WHERE id = ?;", session["user_id"])
            cash = rows[0]["cash"]
            new_cash = cash - total
            if new_cash < 0:
                return apology("insufficient cash", 400)

            # Add transaction
            transaction_id = db.execute(
                "INSERT INTO transactions (user_id, symbol, name, shares, price, total, timestamp) VALUES (?, ?, ?, ?, ?, ?, datetime());",
                session["user_id"], quotation["symbol"], quotation["name"], shares, price, total)
            # insert current time with `datetime()` function

            # Debit amount
            user_id = db.execute(
                "UPDATE users SET cash = ? WHERE id = ?;", new_cash, session["user_id"])

        except RuntimeError:
            # what if transaction is successful but debit fails?
            # but user_id is same in both so both should error
            return apology("couldn't record transaction in database", 500)

        flash("Bought!")

        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    COLUMNS = {"Symbol": 0, "Shares": 2, "Price": 2, "Timestamp": 2}
    COLUMN_NAMES, ALIGNMENT = list(COLUMNS.keys()), list(COLUMNS.values())

    # using ? substitution places quotes which are invalid for column names
    transactions = db.execute(
        f"SELECT {','.join(COLUMN_NAMES)} FROM transactions WHERE user_id = ? ORDER BY id",
        session["user_id"])

    data = {
        "columns": COLUMN_NAMES,
        "alignment": ALIGNMENT,
        "transactions": transactions,
        "usd_fields": list([k.lower() for k in COLUMN_NAMES][-2:-1])
    }

    return render_template("history.html", **data)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 400)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?",
                          request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 400)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        symbol = request.form.get("symbol")

        if not symbol:
            return apology("must provide symbol", 400)

        # Lookup symbol
        quotation = lookup(symbol)

        if quotation is None:
            return apology("invalid symbol", 400)

        return render_template("quoted.html", **quotation)

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Extract form input
        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")

        # Ensure username was submitted
        if not username:
            return apology("must provide username", 400)

        # Ensure password was submitted
        elif not password:
            return apology("must provide password", 400)

        # Ensure confirm was submitted
        elif not confirmation:
            return apology("must provide password confirmation", 400)

        # Ensure both passwords match
        elif password != confirmation:
            return apology("passwords don't match", 400)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?",
                          username)

        # Ensure username doesn't exist
        if rows:
            return apology("username already exists", 400)

        password_hash = generate_password_hash(password)

        # Add user
        try:
            rows = db.execute(
                "INSERT INTO users (username, hash) VALUES (?, ?)", username,
                password_hash)
        except RuntimeError:
            return apology("couldn't add user information into database", 500)

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "POST":
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")

        if not symbol:
            return apology("missing symbol", 400)
        if not shares:
            return apology("missing shares", 400)
        try:
            shares = int(shares)
            if shares < 1:
                raise ValueError
        except (ValueError, TypeError):
            return apology("invalid shares value", 400)

        try:
            assets, _, _ = get_assets(session["user_id"])
        except RuntimeError:
            return apology("could not sell stock", 500)

        owned_shares = {s: assets[s]["shares"] for s in assets}

        if symbol not in owned_shares:
            return apology("symbol not owned", 400)
        if shares > owned_shares[symbol]:
            return apology("too many shares", 400)

        # Lookup symbol
        quotation = lookup(symbol)

        if quotation is None:
            return apology("invalid symbol", 400)

        # Record negative value of shares
        shares = -shares

        price = quotation["price"]

        total = shares * price

        try:
            # Get user cash
            cash = db.execute("SELECT cash FROM users WHERE id = ?;", session["user_id"])[
                0]["cash"]
            new_cash = cash - total  # total is negative

            # Add transaction
            transaction_id = db.execute(
                "INSERT INTO transactions (user_id, symbol, name, shares, price, total, timestamp) VALUES (?, ?, ?, ?, ?, ?, datetime());",
                session["user_id"], quotation["symbol"], quotation["name"], shares, price, total)
            # insert current time with `datetime()` function

            # Update cash
            user_id = db.execute(
                "UPDATE users SET cash = ? WHERE id = ?;", new_cash, session["user_id"])

        except RuntimeError:
            return apology("couldn't record transaction in database", 500)

        flash("Sold!")

        return redirect("/")

    elif request.method == "GET":
        try:
            rows = db.execute(
                "SELECT DISTINCT symbol FROM transactions WHERE user_id = ?;",
                session["user_id"])
            symbols = [r["symbol"] for r in rows]
        except RuntimeError:
            return apology("could not retrieve transactions", 500)

        return render_template("sell.html", symbols=symbols)


@app.route("/info")
def info():
    """View descriptive information page"""
    FILE_PATH = "templates/info.md"
    with open(FILE_PATH, 'r') as md_file:
        html = render_markdown(md_file.read())
    # static info template
    return render_template("info.html", content=html)


def get_assets(user_id):
    """Return net assets based on transactions"""
    COLUMNS = ["symbol", "name", "shares", "price", "total"]
    # !! same keys are used in js,css
    SYMBOL_INFO_DEFAULT = {
        "symbol": "",
        "name": "",
        "shares": 0,
        "price": 0.0,
        "price_diff_pct": "",
        "total": 0.0}
    DEL_COLUMNS = []

    # using ? substitution places quotes which are invalid for column names
    transactions = db.execute(
        f"SELECT {','.join(COLUMNS)} FROM transactions WHERE user_id = ?",
        user_id)

    # Get user's cash
    cash = db.execute("SELECT cash FROM users WHERE id = ?;",
                      user_id)[0]["cash"]

    assets, _assets = {}, {}  # dict of dicts
    total = cash

    # Populate _assets
    for t in transactions:
        symbol = t["symbol"]
        if symbol not in _assets:
            _assets[symbol] = copy(SYMBOL_INFO_DEFAULT)
            _assets[symbol]["symbol"] = symbol
        _assets[symbol]["shares"] += t["shares"]
        _assets[symbol]["total"] += t["total"]

    for symbol in _assets:
        # Delete invalid symbols
        if _assets[symbol]["shares"] == 0:
            continue
        # Add valid symbols
        assets[symbol] = _assets[symbol]

    # Add price and total
    for symbol in assets:
        info = assets[symbol]

        # (Average) Old price
        price = info["total"] / info["shares"]

        info.update(lookup(symbol))  # update `info` dict

        # Update total worth (updated)
        info["total"] = info["price"] * info["shares"]

        # Calculate price diff
        # (new - old) / old * 100
        price_diff_pct = (info["price"] - price) / info["price"] * 100
        # !! same prefix is used in js,css
        if price_diff_pct == 0:
            info["price_diff_pct"] = "~"
        else:
            prefix = "+" if price_diff_pct > 0 else "-"
            info["price_diff_pct"] = f"{prefix}{abs(price_diff_pct):.2f}%"

        assets[symbol] = info

        # Add to running total
        total += info["total"]

    # Clean up; remove unnecessary columns
    for symbol in assets:
        for key in assets[symbol]:
            if key in DEL_COLUMNS:
                del assets[symbol][key]

    return assets, total, cash


def render_markdown(string):
    md = Markdown(
        extensions=["extra", "sane_lists", "smarty", "toc", "pymdownx.arithmatex"],
        # https://facelessuser.github.io/pymdown-extensions/extensions/arithmatex/#input-format
        # ! dont put spaces after the "$" signs
        # otherwise arithmatex will interpret it as a literal dollar sign
        extension_configs={
            "pymdownx.arithmatex": {
                "generic": True,
                "smart_dollar": True
            }
        }
    ).convert
    return md(string)
