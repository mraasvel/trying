#include <iostream>

/* std::is_empty implementation */

namespace ft
{

	namespace detail {

	/*
	Note: need C++11 because of std::true_type, but easy to implement own
	*/
		template <typename T, bool condition>
		struct is_empty_detail : public std::false_type {};

		// Condition is true, therefore T is an empty class
		template <typename T>
		struct is_empty_detail<T, true>
			: public std::true_type {};

		//private inheritance to check size
		template <typename T>
		struct size_check : private T {
			int a;
		};

	}

// todo: check is_fundamental so fundamental types don't error
template <typename T>
struct is_empty
	: public detail::is_empty_detail<T, sizeof(detail::size_check<T>) == sizeof(int)> {};

}

/* Compressed Type implementation */

namespace ft {

	namespace detail {
		template <typename NonEmptyClass, typename EmptyClass>
		class CompressedType : private EmptyClass {
			NonEmptyClass x;
		};
	}

	namespace detail {
		template <typename T1, typename T2, bool T1_isempty, bool T2_isempty>
		class CompressedPairDetail {
		public:
			CompressedPairDetail() {
				std::cout << "Either both are empty or neither" << std::endl;
			}
			T1 x;
			T2 y;
		};

		template <typename T1, typename T2>
		class CompressedPairDetail<T1, T2, true, false> {
		public:
			CompressedPairDetail() {
				std::cout << "T1 is empty" << std::endl;
			}

			CompressedType<T2, T1> x;
		};

		template <typename T1, typename T2>
		class CompressedPairDetail<T1, T2, false, true> {
		public:
			CompressedPairDetail() {
				std::cout << "T2 is empty" << std::endl;
			}
			CompressedType<T1, T2> x;
		};
	}

template <typename T1, typename T2>
class CompressedPair 
	: public detail::CompressedPairDetail<T1, T2, is_empty<T1>::value, is_empty<T2>::value> {
public:
	CompressedPair() {}
};

}


class EmptyClass {};
class FullClass {
	int n;
};

int main() {
	ft::CompressedPair  <EmptyClass,  FullClass> s;
	ft::CompressedPair  < FullClass, EmptyClass> s1;
	ft::CompressedPair  < FullClass,  FullClass> s2;
	ft::CompressedPair  <EmptyClass, EmptyClass> s3;

	std::cout << "Size with  left empty: " <<  sizeof(s) << std::endl;
	std::cout << "Size with right empty: " << sizeof(s1) << std::endl;
	std::cout << "Size with  none empty: " << sizeof(s2) << std::endl;
	std::cout << "Size with  both empty: " << sizeof(s3) << std::endl;
	return 0;
}
