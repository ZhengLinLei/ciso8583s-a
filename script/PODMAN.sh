cd ../

P=./dist/ciso8583_ots_a.tgz

# Check if exist CUSTOM PATH
if [ "$#" -eq "1" ]; then
    P=$1
    echo "Importing project from: PATH > $P"
fi

gzip -fd $P || {
    echo "Error in: PATH > $P"
    exit 1
}

NAME="${P%.*}"
container_name=ciso8583_ots_a

# STOP 
./script/STOP.sh podman || {
    echo "Default remove"
    podman container rm $container_name
}

# IMAGE
echo "Checking image $container_name"
podman image exists $container_name
if [ "$?" -eq "1" ]; then
    echo "Removing image $container_name"
    podman image rm $container_name
fi

echo "Loading image copy"
podman image load -i $NAME.tar

# RUN IMAGE
podman run -dit --name $container_name -p 9101:9101 -v /etc/localtime:/etc/localtime:ro -v ciso8583_ots_a_logs:/opt/Ciso8583/log $container_name
