cat words.js | tr , '\n' | sed 's/^ *//' | tr -d \" | sed '/^[[:space:]]*$/d' > words.txt
