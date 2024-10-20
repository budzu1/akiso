#!/bin/bash

distro_det()
{
    info_path="/etc/os-release"

    if [ -e "$info_path" ]; then

        source "$info_path" # in this file we have some vars an now they are accessible we need ID

        case $ID in
            ubuntu|debian|linuxmint)
                PKG_UPDATE="apt-get update"
                PKG_INSTALL="apt-get install -y" # -y is to auto say yes during installation
                ;;

            fedora)
                PKG_UPDATE="dnf update -y"
                PKG_INSTALL="dnf install -y"
                ;;

            arch)
                PKG_UPDATE="pacman -Syu --noconfirm"
                PKG_INSTALL="pacman -Sy --noconfirm"
                ;;

            opensuse-leap)
                PKG_UPDATE="zypper refresh"
                PKG_INSTALL="zypper install -y"
                ;;

            *) 
                echo "id unknown: $ID"
                exit 1
                ;;
        esac

    else 
    
        echo "problem with file"
        exit 2;

    fi

    export PKG_UPDATE
    export PKG_INSTALL
}

distro_det