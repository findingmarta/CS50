import datetime
import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

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

    # Get user's purchases
    purchases = db.execute("SELECT * FROM purchases WHERE user_id = ? GROUP BY name", session["user_id"])

    # If the user has at least one purchase
    if len(purchases) != 0:
        # Get the total of money used by the user
        user_cash = db.execute("SELECT SUM(total) AS cash FROM purchases WHERE user_id = ?", session["user_id"])

        # Get the current balance
        cash = 10000 - user_cash[0]['cash']

        # Get the total
        sum = cash + user_cash[0]['cash']
    # If the user doesn't have a purchase
    else:
        cash = 10000
        sum = 10000

    return render_template("index.html", purchases=purchases, cash=cash, sum=sum)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure symbol was submitted
        symbol = request.form.get("symbol").upper()
        if not symbol:
            return apology("missing symbol", 400)

        # Ensure shares was submitted
        shares = request.form.get("shares")
        if not shares:
            return apology("missing shares", 400)

        # Ensure shares was an integer
        if not shares.isdigit():
            return apology("invalid shares", 400)

        # Ensure symbol exists
        if lookup(symbol) == None:
            return apology("invalid symbol", 400)

        # Saves the user's id
        id = session["user_id"]

        # Saves the current price of the stock
        price = lookup(symbol)["price"]

        # Select the user's current balance
        balance = db.execute("SELECT cash FROM users WHERE id = ?", id)

        # Get the total of the purchase
        total = int(shares) * price

        # Ensure that the user can afford the number of shares at the current price
        if total > balance[0]['cash']:
            return apology("can't afford", 400)

        # Change database's data
        name = lookup(symbol)["name"]
        transac = datetime.datetime.now()
        rows = db.execute("SELECT * FROM purchases WHERE user_id = ? AND symbol=?", id, symbol)
        if len(rows) != 0:
            # Update the entry already existent
            new_shares = int(shares) + int(rows[0]['shares'])
            new_total = new_shares * price
            db.execute("UPDATE purchases SET shares = ?, total = ? WHERE user_id = ? AND symbol = ?", new_shares, new_total, id, symbol)
        else:
            # Insert new entry
            db.execute("INSERT INTO purchases (symbol, name, shares, price, total, user_id) VALUES(?, ?, ?, ?, ?, ?)",
                       symbol, name, shares, price, total, id)

        # Insert data into history
        db.execute("INSERT INTO history (transac, transac_type, user_id, purchase_symbol, purchase_shares, purchase_price) VALUES(?, ?, ?, ?, ?, ?)",
                   transac, "B", id, symbol, shares, price)

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    # Get data
    history = db.execute("SELECT * FROM history WHERE user_id = ? ORDER BY transac", session["user_id"])
    return render_template("history.html", history=history)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

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


@app.route("/change_password", methods=["GET", "POST"])
def change():
    """Change user's password"""

    # Forget any user_id
    # session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Ensure password was submitted
        password = request.form.get("password")
        if not password:
            return apology("must provide password", 400)

        # Ensure password matches the password-confirmation
        if request.form.get("password") != request.form.get("confirmation"):
            return apology("passwords don't match", 400)

        # Change password
        db.execute("UPDATE users SET hash = ? WHERE id = ?", generate_password_hash(password), session["user_id"])

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("change.html")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure symbol was submitted
        symbol = request.form.get("symbol")
        if not symbol:
            return apology("missing symbol", 400)

        # Ensure symbol exists
        if lookup(symbol) == None:
            return apology("invalid symbol", 400)

        # Saves the dict returned by the function lookup
        response = lookup(symbol)
        return render_template("quoted.html", response=response)

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

        # Ensure username was submitted
        username = request.form.get("username")
        if not username:
            return apology("must provide username", 400)

        # Ensure password was submitted
        password = request.form.get("password")
        if not password:
            return apology("must provide password", 400)

        # Ensure password matches the password-confirmation
        if request.form.get("password") != request.form.get("confirmation"):
            return apology("passwords don't match", 400)

        # Ensure that the username doesn't already exists
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))
        if len(rows) != 0:
            return apology("username already taken", 400)

        # Insert regist into database
        db.execute("INSERT INTO users (username, hash) VALUES(?, ?)", username, generate_password_hash(password))

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    # Saves the user's id
    id = session["user_id"]

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure symbol was submitted
        symbol = request.form.get("symbol").upper()
        item = lookup(symbol)
        if not item:
            return apology("missing symbol", 400)

        # Ensure shares was submitted
        shares = request.form.get("shares")
        if not shares:
            return apology("missing shares", 400)

        # Ensure shares was an integer
        if not shares.isdigit():
            return apology("invalid shares", 400)

        # Ensure shares was a positive number
        if int(shares) == 0:
            return apology("shares must be positive", 400)

        # Ensure that the user sell the correct number of shares
        stock_shares = db.execute("SELECT shares FROM purchases WHERE symbol = ?", symbol)
        if int(shares) > int(stock_shares[0]['shares']):
            return apology("too many shares", 400)

        # Saves the current price of the stock
        price = lookup(symbol)["price"]

        # Saves current date and time
        transac = datetime.datetime.now()

        # Change database's data
        if int(shares) < int(stock_shares[0]['shares']):
            # Update the entry already existent
            new_shares = int(stock_shares[0]['shares']) - int(shares)
            new_total = new_shares * price
            db.execute("UPDATE purchases SET shares = ?, total = ? WHERE user_id = ? AND symbol = ?", new_shares, new_total, id, symbol)
        else:
            # Delete entry
            db.execute("DELETE FROM purchases WHERE symbol = ?", symbol)

        # Update the table history
        db.execute("INSERT INTO history (transac, transac_type, user_id, purchase_symbol, purchase_shares, purchase_price) VALUES(?, ?, ?, ?, ?, ?)",
                   transac, "S", id, symbol, shares, price)

        # Redirect the user to the home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        # Get the symbol of the user's current stocks
        symbols = db.execute("SELECT symbol FROM purchases WHERE user_id = ?", id)
        return render_template("sell.html", symbols=symbols)
