class A{
	public a = 0;
	public static s = 1;

	function init(a){
		this.a = a;
		print 'A init', a;
	}

	function f(a, b=1){
		return a + b;
	}
}

print A.s; // 1
a = new A(1); // A init 1
print a.f(1, 2);
