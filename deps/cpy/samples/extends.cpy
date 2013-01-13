class A{
	function f(){
		print 'A.f';
	}
}
class B extends A{
	function g(){
		print "B.g";
	}
}

b = new B();
b.f();
b.g();
