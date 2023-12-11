# This script contains
#
#   ./BUILD.sh with export to ./dist/
#   ./RUN.sh the project
#   ./STOP.sh the project when exit

# Move to script folder
cd ./script

DATETIME=$(date '+%Y%m%d_%H%M%S')
./BUILD.sh ./dist/ciso8583_ots_a_$DATETIME.tgz

# Echo how to export to production
echo -e "\n\n\033[94m------------------------------------"
echo -e "The project image is located in ./dist/"
echo -e "Follow the instruction in ./PODMAN.txt to import the image to production"
echo -e "\n"
echo -e "Example:"
echo -e "   
    cd dist
    gzip -d ./ciso8583_ots_a.tgz \n \
    ../script/STOP.sh podman \n \
    podman image rm ciso8583_ots_a \n \
    podman image load -i ./ciso8583_ots_a.tar \n \
    podman run -dit --name ciso8583_ots_a -p 9101:9101 -v /etc/localtime:/etc/localtime:ro -v ciso8583_ots_a_logs:/opt/Ciso8583/log ciso8583_ots_a
    
"
echo -e "------------------------------------\n\n\033[0m"

read -p "Do you want to run IMAGE? (y/n) " -n 1 -r
echo
if [[ $REPLY =~ ^[Yy]$ ]]; then
    ./RUN.sh


    # If user send kill sign
    ./STOP.sh
fi


echo "---- Finished! ----"