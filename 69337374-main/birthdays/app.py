from string import Template as Tpl

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///birthdays.db")

# Constants
MONTHS = ["January", "February", "March", "April", "May", "June",
          "July", "August", "September", "October", "November", "December"]
ORDINALS = {"1": "st", "2": "nd", "3": "rd", "0456789": "th"}
ADD_STATUSES = [
    Tpl("Birthday #$id was added successfully!"),
    Tpl("Birthday information was invalid. Try again.")
]
DEL_STATUSES = [
    Tpl("Birthday #$id was deleted successfully!"),
    Tpl("Birthday deletion request for #$id was invalid. Try again.")
]
# -1 = ..., 0 = ✔️, 1 = ❌
ADD_STATUS_CODE = -1
DEL_STATUS_CODE = -1

ADD_ID = -1
DEL_ID = -1


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/", methods=["GET", "POST"])
def index():
    global ADD_STATUSES, ADD_STATUS_CODE, DEL_STATUSES, DEL_STATUS_CODE, ADD_ID, DEL_ID

    if request.method == "POST":
        form_type = request.form.get("type")

        # ADD FORM SUBMITTED
        if form_type == "add":
            print("➕ add form submitted", flush=True)

            name = request.form.get("name")
            day = request.form.get("day")
            month = request.form.get("month")

            try:
                day, month = int(day), int(month)
                basic_check = (name) and True
                date_check = (month == 2 and day <= 29) or \
                    (month in [4, 6, 9, 11] and day <= 30) or \
                    (month in [1, 3, 5, 7, 8, 10, 12] and day <= 31)
                if not (basic_check and date_check):
                    raise ValueError

                # raises RuntimeError
                add_id = db.execute("INSERT INTO birthdays (name, day, month) VALUES (?, ?, ?)", name, day, month)
                ADD_STATUS_CODE = 0
                ADD_ID = add_id
                print(f"✔️ valid birthday added:\nname=`{name}`, " +
                      f"day=`{day}`, month=`{month}`", flush=True)

            except (ValueError, TypeError, RuntimeError):
                ADD_STATUS_CODE = 1
                print(f"❌ invalid birthday not added:\nname=`{name}`, " +
                      f"day=`{day}`, month=`{month}`", flush=True)

        # DELETE FORM SUBMITTED
        elif form_type == "del":
            print("➖ del form submitted", flush=True)
            delete_id = request.form.get("delete-id")
            try:
                db.execute("DELETE FROM birthdays WHERE id=?;", delete_id)
                DEL_STATUS_CODE = 0
                DEL_ID = delete_id
                print(f"✔️ successfully deleted birthday id: {delete_id}", flush=True)
            except RuntimeError:
                DEL_STATUS_CODE = 1
                print(f"❌ unsuccessful birthday not deleted id: {delete_id}", flush=True)

        # REDIRECT
        return redirect("/")

    elif request.method == "GET":
        birthdays = db.execute("SELECT * FROM birthdays;")
        for b in birthdays:
            month_string = MONTHS[b["month"] - 1]
            day_string = str(b["day"])

            for digits in ORDINALS:
                if day_string[-1] in digits:
                    day_string += ORDINALS[digits]

            b["day_string"] = f"{day_string} {month_string}"

        add_status, ADD_STATUS_CODE = get_status(ADD_STATUSES, ADD_STATUS_CODE, True, id=ADD_ID)
        del_status, DEL_STATUS_CODE = get_status(DEL_STATUSES, DEL_STATUS_CODE, True, id=DEL_ID)

        return render_template("index.html", birthdays=birthdays,
                               add_status=add_status, del_status=del_status)


def get_status(statuses, code, reset=True, **substitution):
    status = ""  # default status
    if code >= 0:
        status_template = statuses[code]  # get status
        status = status_template.substitute(**substitution)
    print("i:", status, code, reset, substitution, flush=True)
    code = -1 if reset else code  # this does not reset global code
    print("f:", status, code, reset, substitution, flush=True)
    return status, code
