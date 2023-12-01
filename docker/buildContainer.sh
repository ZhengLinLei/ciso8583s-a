# GCC Make project
cd src/

make

cp -a ../dist/. ../tmp/
# Add full access to be remove by anyone
chmod 777 ../tmp
chmod 777 ../tmp/ciso8583

make clean

cd -

echo "Done"