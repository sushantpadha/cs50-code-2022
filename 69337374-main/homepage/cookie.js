window.onload = function () {
    var num_cookies = 0;
    const cookie = document.getElementById("cookie");
    const cookie_count = document.getElementById("cookie-count");
    cookie.addEventListener("click", (e) => {
        num_cookies += 1;
        cookie_count.textContent = num_cookies.toString();
    })
}
