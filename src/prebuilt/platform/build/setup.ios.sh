#! /bin/bash

cd $(dirname "$0")
cd ..

#plugin
cd pluginx
sh clean.ios.plugins.sh
sh setup.ios.plugins.sh
cd ..

cd ios_sh
sh setup.ios.official.sh
# sh setup.ios.uc.sh
# sh setup.ios.appstore.sh
# sh setup.ios.downjoy.sh
# sh setup.ios.n91.sh
