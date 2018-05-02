#ifndef GUARD_SPPLIT
#define GUARD_SPIT

#include "sstream"
#include "tuple"
#include "iostream"
#include "type_traits"
#include "string"

namespace splitter {

	template<typename... Types>
	class MyType {
	public:
		// in this case && is universal (forwarding) reference
		MyType(Types &&... params) :pack(params...) {}

		std::tuple<Types...> pack;


	};

}

namespace splitter_impl {

	

	//basic state, just with one type left -> means all type has been already compared
	template<typename T, typename... Args>
	struct check {};


	// cut first two types and call the same just with second type and rest types
	template<typename T, typename U, typename... Args> struct check<T, U, Args...> : check<U, Args...> {

		static_assert(!(std::is_lvalue_reference<T>::value && std::is_lvalue_reference<U>::value), "2 adjacent lvalues");
		static_assert(!(!std::is_lvalue_reference<T>::value && !(std::is_same<T, char>::value)), "delimiter is not char !");


	};


	//if empty
	template<std::size_t I, typename...Rest>
	struct Reader {

		template<typename...T2>
		static void process_arguments(std::istream& is, const splitter::MyType<T2...>& p) {}

	};


	//last one is lvalue
	template<std::size_t I, typename T>
	struct Reader<I, T> {

		template<typename ... T2>
		static void process_arguments(std::istream& is, const splitter::MyType< T2 ...> & p) {

			std::string value;
			//implicit read while end of line \n
			std::getline(is, value);
			std::istringstream ss(value);
			ss >> std::get<I>(p.pack);
			//check if we eat up all characters from ss
			if (!ss.eof()) {
				throw std::logic_error("expected delimiter is not occur in the text");
			}
		}
	};

	//last one is delimiter char
	template<std::size_t I>
	struct Reader<I, char> {

		template<typename ... T2>
		static void process_arguments(std::istream& is, const splitter::MyType< T2 ...> & p) {
			char c;
			is.get(c);
			if (c != std::get<I>(p.pack))
				throw std::logic_error("delimiter is not same as expected");

		}
	};

	//if current delimiter is not last  and don't have preceding lvalue parameter
	template<std::size_t I, typename...Rest>
	struct Reader<I, char, Rest...> {

		template<typename ... T2>
		static void process_arguments(std::istream& is, const splitter::MyType< T2 ...> & p) {
			char c;
			is.get(c);
			if (c != std::get<I>(p.pack))
				throw std::logic_error("delimiter is not same as expected");

			Reader<I + 1, Rest...>::process_arguments(is, p);
		}
	};

	//choose only if T is lvalue because of type check before SEE : struct Check 
	template<std::size_t I, typename T, typename...Rest>
	struct Reader<I, T, char, Rest...> {

		template<typename ... T2>
		static void process_arguments(std::istream& is, const splitter::MyType< T2 ...> & p) {

			std::string value;			
			std::getline(is, value, std::get<I + 1>(p.pack));

			if (is.eof() || is.fail() || is.bad())
				throw std::logic_error("expected delimiter is not occur in the text");
			

			std::istringstream ss(value);
			
			ss >> std::get<I>(p.pack);	//if read to char(T is char) eof bit is not set
			ss.peek();					//check if there is more character to read if not sets eof bit

			//check if we eat up all characters from ss
			if (!ss.eof())
				throw std::logic_error("expected delimiter is not occur in the text");
			

			Reader<I + 2, Rest...>::process_arguments(is, p);





		}
	};
}

namespace splitter {

	

	template<typename... Types>
	inline MyType<Types...> split(Types && ... params) {

		splitter_impl::check<Types...> check_types;
		
		//std::forward respect the referencness instead std::move make rvalues from all
		return MyType<Types...>(std::forward<Types>(params)...);
	}

	template<typename...Types>
	inline std::istream&  operator>>(std::istream& is,const MyType<Types...>& c) {

		//start on the index 0
		splitter_impl::Reader<0, Types...>::process_arguments(is, c);


		return is;
	}
}

	

	











#endif // !GUARD_SPPLIT

