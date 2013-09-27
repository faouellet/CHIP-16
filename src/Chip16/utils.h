#ifndef UTILS_H
#define UTILS_H

/**
* \namespace Utils
* \brief Contains a mix of useful templated tools.
*        Credits goes to Patrice Roy for teaching me some of those tools
*        His website : <a href="http://h-deb.clg.qc.ca/">h-deb</a>
*/
namespace Utils
{
	template <bool Cond, class If, class Else>
	struct IfElse;

	template <class If, class Else>
	struct IfElse<true, If, Else>
	{
		typedef If type;
	};

	template <class If, class Else>
	struct IfElse<false, If, Else>
	{
		typedef Else type;
	};

	class Error;
	template<int N>
	struct UInt_
	{
		typedef typename IfElse<
			sizeof(char)==N, unsigned char,
				typename IfElse<
					sizeof(short)==N, unsigned short,
					typename IfElse<
						sizeof(int)==N, unsigned int,
						typename IfElse<
							sizeof(long)==N, unsigned long,
							typename IfElse<
								sizeof(long long)==N, unsigned long long,
								Error
							>::type
						>::type
					>::type
				>::type
		>::type type;
	};

	typedef UInt_<1>::type UInt8;
	typedef UInt_<2>::type UInt16;
	typedef UInt_<4>::type UInt32;

	template<class T>
	struct LeftShift
	{
		T operator()(const T & in_LHS, const UInt16 & in_RHS)
		{
			return in_LHS << in_RHS;
		}
	};

	template<class T>
	struct RightShift
	{
		T operator()(const T & in_LHS, const UInt16 & in_RHS)
		{
			return in_LHS >> in_RHS;
		}
	};
};

#endif // UTILS_H
