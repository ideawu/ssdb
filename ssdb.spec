Name:		ssdb
Version:	1.6.8.5
Release:	1%{?dist}
Summary:	A fast NoSQL database, an alternative to Redis.

Group:		Applications/Productivity
License:	BSD
URL:		https://github.com/ideawu/ssdb
Source0:	%{name}-%{version}.tgz
#BuildRoot:	%(mktemp -ud %{_tmppath}/%{name}-%{version}-%{release}-XXXXXX)
NoSource:	0
BuildRequires:	gcc, make
Requires:	/sbin/chkconfig /usr/sbin/useradd

%description
SSDB is a high performace key-value(key-string, key-zset, key-hashmap) NoSQL database, an alternative to Redis.

%prep
%setup -q
#%patch1 -p1

%build
make %{?_smp_mflags} DESTDIR=%{buildroot}

%install
make %{?_smp_mflags} DESTDIR=%{buildroot} install

%clean
rm -rf %{buildroot}


%files
%defattr(-,nobody,nobody,-)
/usr/local/ssdb

%doc


%changelog
* Thu Apr 24 2014 @ 22:47:29 # higkoohk@gmail.com
- 编写ssdb的rpm编译源文件
