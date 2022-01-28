cat words.js | tr , '\n' | tr -d \" | sed '/^[[:space:]]*$/d' > words.txt
