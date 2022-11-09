#!/bin/bash_shellshock

render_results() {
    echo "Content-Type: text/html"
    echo ""
    echo ""
    echo "<style>"
    echo "body {"
    echo "background-color: #AFD5FC;"
    echo "}"
    echo "h1 {"
    echo "text-align: center;"
    echo "}"
    echo "p {"
    echo "text-align: center;"
    echo "}"
    echo "#dlobeid-etovucca-voting-machine {"
    echo "font-size: 18px;"
    echo "}"
    echo "</style>"
    echo "<link rel='stylesheet' href='https://spar.isi.jhu.edu/teaching/443/main.css'>"
    echo "<body>"
    echo "<p> Election Results: </p>"
    echo "<p><a href='./home.cgi'>Return home</a></p>"
    echo "</body>"
}

render_results