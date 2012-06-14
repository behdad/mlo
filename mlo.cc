namespace MetaLoopOptimizer {

	struct Null  {};


	/* Constants */

	template <typename T, int I>
	struct Constant { typedef T return_type; static const T value = I; };

	template <bool I> struct Bool : Constant<bool, I> {};
	template <short I> struct Short : Constant<int, I> {};
	template <unsigned short I> struct UShort : Constant<unsigned int, I> {};
	template <int I> struct Int : Constant<int, I> {};
	template <unsigned int I> struct UInt : Constant<unsigned int, I> {};
	template <long I> struct Long : Constant<long, I> {};
	template <unsigned long I> struct ULong : Constant<unsigned long, I> {};

	typedef Bool<false> False;
	typedef Bool<true> True;

	typedef Int<0> Zero;
	typedef Int<1> One;
	typedef Int<2> Two;

	typedef UInt<0> UZero;
	typedef UInt<1> UOne;
	typedef UInt<2> UTwo;


	/* Data structures */

	struct List
	{
		private:
		template <typename Head, typename Tail>
		struct Node {
			typedef Head data;
			typedef Tail next;
		};
		public:

		template <typename List, typename Data>
		struct prepend {
			typedef Node<Data, List> value;
		};

		template <typename List, typename Data>
		struct append {
		};
		template <typename Data>
		struct append<Null, Data> {
			typedef Node<Data, Null> value;
		};
		template <typename Data, typename THead, typename TTail>
		struct append<List::Node<THead, TTail>, Data> {
			typedef Node<THead, typename append<TTail, Data>::value> value;
		};

		template <typename List, typename Member>
		struct has {
		};
		template <typename Member>
		struct has<Null, Member> {
			static inline bool call (void) { return false; }
		};
		template <typename THead, typename TTail>
		struct has<Node<THead, TTail>, THead> {
			static inline bool call (void) {
				return true;
			}
		};
		template <typename THead, typename TTail, typename Member>
		struct has<Node<THead, TTail>, Member> {
			static inline bool call (void) {
				return has<TTail, Member>::call ();
			}
		};

		template <typename List, template <typename THead> class Func>
		struct apply {
		};
		template <template <typename THead> class Func >
		struct apply<Null, Func> {
			static inline void call (void) {}
		};
		template <typename THead, typename TTail, template <typename TTHead> class Func>
		struct apply<Node<THead, TTail>, Func> {
			static inline void call (void) {
				Func<THead>::call ();
				apply<TTail, Func>::call ();
			}
		};
	}; // List

	struct Expr
	{
		template <typename  T, int V>
		struct Const {
			typedef T return_type;
			static const T value = V;
		};
		template <typename A, typename B>
		struct Add {
			typedef typename A::return_type return_type;
			static const return_type value = A::value + B::value;
		};
		template <typename A, typename B>
		struct Sub {
			typedef typename A::return_type return_type;
			static const return_type value = A::value - B::value;
		};
		template <typename A, typename B>
		struct Mul {
			typedef typename A::return_type return_type;
			static const return_type value = A::value * B::value;
		};
		template <typename A, typename B>
		struct Div {
			typedef typename A::return_type return_type;
			static const return_type value = A::value / B::value;
		};
		template <typename A, typename B>
		struct Mod {
			typedef typename A::return_type return_type;
			static const return_type value = A::value % B::value;
		};
		template <typename Test, typename True, typename False>
		struct Cond {
			typedef typename True::return_type return_type;
			static const return_type value = Test::value ? True::value : False::value;
		};
		template <typename First, typename Second>
		struct Comma {
			typedef typename Second::return_type return_type;
			static const return_type value = (First::value, Second::value);
		};
	}; // Expr

	struct Stmt
	{
	};

} // MetaLoopOptimizer


using namespace MetaLoopOptimizer;

#include <stdio.h>

struct A { static const char name = 'A'; };
struct B { static const char name = 'B'; };
struct C { static const char name = 'c'; };

template <typename T>
struct Print {
	static inline void call (void) { printf ("%c\n", T::name); }
};

int
main (void)
{
	typedef List::prepend<List::prepend<List::prepend<Null, A>::value, B>::value, A>::value my_list;
	typedef List::append<my_list, C>::value new_list;

	List::apply<my_list, Print>::call ();
	printf ("\n");
	List::apply<new_list, Print>::call ();
	printf ("\n");

	if (List::has<my_list, C>::call ())
		printf ("my_list has C\n");
	if (List::has<new_list, C>::call ())
		printf ("new_list has C\n");


	typedef Expr::Cond<False, One, Zero> false_cond;
	printf ("false %d\n", false_cond::value);
	printf ("1+2 %d\n", Expr::Add<One, UTwo>::value);
}
