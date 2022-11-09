#!/bin/bash

render_home() {
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
    echo "<h2 id='dlobeid-etovucca-voting-machine'>DLOBEID EtovUcca Voting Machine</h2>"
    echo "<h1 id='home'>Home</h1><br>"
    echo "<h1> Welcome to the Voting Machine! </h1>"
    echo "<p>"
    echo "<a href='./register.cgi'>Register to Vote</a>"
    echo "<li><a href='./vote.cgi'>Vote for an Office</a>"
    echo "<a href='./login.cgi'>Administrator Interface (Requires Login)</a>"
    echo "</p>"
    echo "</body>"
}

render_home