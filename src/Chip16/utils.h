#ifndef UTILS_H
#define UTILS_H

/**
* \namespace Utils
* \brief Contains a mix of useful templated tools.
*        Credits goes to Patrice Roy for teaching me those tools
*        His website : <a href="http://h-deb.clg.qc.ca/">h-deb</a>
*/
namespace Utils
{
	template <bool Cond, typename If, typename Else>
	struct IfElse;

	template <typename If, typename Else>
	struct IfElse<true, If, Else>
	{
		typedef If type;
	};

	template <typename If, typename Else>
	struct IfElse<false, If, Else>
	{
		typedef Else type;
	};

	class Error;
	template<int N>
	struct Int_
	{
		typedef typename IfElse<
			sizeof(char)==N, char,
				typename IfElse<
					sizeof(short)==N, short,
					typename IfElse<
						sizeof(int)==N, int,
						typename IfElse<
							sizeof(long)==N, long,
							typename IfElse<
								sizeof(long long)==N, long long,
								Error
							>::type
						>::type
					>::type
				>::type
		>::type type;
	};

	typedef Int_<1>::type UInt8;
	typedef Int_<2>::type UInt16;
	typedef Int_<4>::type UInt32;
};

#endif // UTILS_H
