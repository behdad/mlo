namespace MetaLoopOptimizer {

	struct Null {
		typedef Null value;
	};

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
}
