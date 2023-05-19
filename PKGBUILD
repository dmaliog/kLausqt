# Maintainer: Dmali <dmali@vk.com>

pkgname=klaus-bin
pkgver=1.1
_releasedate=2023-05-19
pkgrel=1
pkgdesc="Arch manager that allows you to download, install, delete packages (including PKGBUILD changes) and display ratings and other things. Add/use automatic scripts to automate repeated actions. Add kernel modules and boot time to GRUB. View system logs. Enable auto-update of the system, delete orphans, and the package cache (with its configuration). There are recommended applications that allow you to install useful applications without problems."
arch=('x86_64')
url="https://github.com/dmaliog/kLausqt/"
license=('GPL 2.0')
groups=()
depends=('qt5-base' 'qt5-multimedia' 'qt5-webengine' 'qt5-xmlpatterns' 'yay' 'zenity' 'libnotify' 'konsole' 'neofetch')

source=(
	"${pkgname}-${pkgver}.zip::${url}/releases/download/${pkgver}/klaus-${pkgver}-${_releasedate}-linux64.zip"
)

sha256sums=(
	'9273a9c1548d8ba8b4b8e5e9c6f459c3dc8bd4c0db4b34040cd2bc05d6a44e1c'
)

package() {
    mkdir -p ${pkgdir}/usr/bin/
    mkdir -p ${pkgdir}/usr/share/icons/hicolor/128x128/apps/

    install -Dm755 ./klaus/klauspro ${pkgdir}/usr/bin/klauspro

	install -Dt ${pkgdir}/usr/share/applications/ -m644 ./klaus/klaus.desktop
	install -Dt ${pkgdir}/usr/share/icons/hicolor/128x128/apps/ -m644 ./klaus/klaus.png
}
