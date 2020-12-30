#! /bin/sh

# Check if all dependencies installed
prog_missing=0
# for prog in python php-cgi
for prog in python
do
if !(which $prog) >/dev/null 2>/dev/null; then
  # printf "\e[1;31mNeed to instal $prog\e[0m\n"
  printf "Need to instal $prog\n"
  prog_missing=1
fi
done
if [ "$prog_missing" -eq "1" ]; then
  exit 1
fi

cat webserv.conf.template |
sed 's=PWD='"$PWD"'=g' |
# sed 's=PHP_CGI='$(which php-cgi)'=g' |
sed 's=PYTHON='$(which python)'=g' > $1
# printf "\e[1;36mDefault config file $1 created.\e[0m\n"
