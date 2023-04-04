#! /bin/sh

# Execute this as: ./hash_script.sh | tee hash_script.log

FILE="connect4_7x6.dat.xz"

echo
stat -c "Hashing file: %n (%s bytes)" $FILE
echo

for rep in `seq 2` ; do
    for hashtool in md5sum sha1sum sha224sum sha256sum sha384sum sha512sum ; do
            /usr/bin/time -f "TIME %C -- wallclock %e kernel %S user %U" $hashtool $FILE 2>&1
    done
    echo
done
