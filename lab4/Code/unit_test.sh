for file in ./../Test/sem*.cmm; do
    echo -e "\033[34mResult of "$file": \033[0m\c"
    ./../parser $file
done