FROM gentoo/stage3-amd64
MAINTAINER gaoliang <lixu19890724@163.com>
RUN rm /sbin/unix_chkpwd
# ADD http://mirrors.tuna.tsinghua.edu.cn/gentoo/snapshots/portage-latest.tar.bz2 /
COPY portage-latest.tar.bz2 /
RUN tar xjf portage-latest.tar.bz2 -C /usr/
RUN echo 'FEATURES="-sandbox -usersandbox"' >> /etc/portage/make.conf
RUN echo 'GENTOO_MIRRORS="http://mirrors.tuna.tsinghua.edu.cn/gentoo"' >> /etc/portage/make.conf
RUN eselect profile set default/linux/amd64/17.0 && . /etc/profile && mkdir -p /etc/portage/repos.conf/
RUN ( \
    echo "[DEFAULT]" && \
    echo "main-repo = gentoo" && \
    echo "[gentoo]" && \
    echo "location = /usr/portage" && \
    echo "sync-type = rsync" && \
    echo "sync-uri = rsync://mirrors.tuna.tsinghua.edu.cn/gentoo-portage/" && \
    echo "auto-sync = yes" \
    ) > /etc/portage/repos.conf/gentoo.conf
RUN emerge --sync
RUN mkdir -p /usr/local/portage-crossdev/{profiles,metadata} && \
    echo 'crossdev' > /usr/local/portage-crossdev/profiles/repo_name && \
    echo 'masters = gentoo' > /usr/local/portage-crossdev/metadata/layout.conf && \
    chown -R portage:portage /usr/local/portage-crossdev
RUN mkdir -p /usr/portage/gentoo/metadata && echo "masters = gentoo" > /usr/portage/gentoo/metadata/layout.conf && echo 'MAKEOPTS="-j5"' >> /etc/portage/make.conf
RUN echo '127.0.0.1 docbook.sourceforge.net' >> /etc/hosts
RUN emerge -1 sys-devel/libtool --quiet-build y && emerge -1 sys-devel/gcc --quiet-build y && emerge -1 sys-devel/binutils --quiet-build y && emerge -1 sys-libs/glibc --quiet-build y && emerge -l crossdev --quiet-build y
RUN crossdev -t aarch64-unknown-linux-gnu
RUN USE="${USE} crossdev" emerge -l distcc --quiet-build y
RUN ( \
    echo "[crossdev]" && \
    echo "location = /usr/local/portage-crossdev" && \
    echo "priority = 10" && \
    echo "masters = gentoo" && \
    echo "auto-sync = no" \
    ) > /etc/portage/repos.conf/crossdev.conf
RUN ( \
    echo "#!/bin/sh" && \
    echo "eval \"\`gcc-config -E\`\"" && \
    echo "exec distccd \"\$@\"" \
    ) > /usr/local/sbin/distccd-launcher && \
    chmod +x /usr/local/sbin/distccd-launcher
RUN rm -r /usr/portage
CMD ["/usr/local/sbin/distccd-launcher", "--allow", "0.0.0.0/0", "--user", "distcc", "--log-level", "notice", "--log-stderr", "--no-detach"]
EXPOSE 3632
