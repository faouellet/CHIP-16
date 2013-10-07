#ifndef UTILS_H
#define UTILS_H

/**
* \namespace Utils
* \brief Contains a mix of useful templated tools.
*        Credits goes to Patrice Roy for teaching me some of those tools
*        His website : <a href="http://h-deb.clg.qc.ca/">h-deb</a>
* \author Felix-Antoine Ouellet
*/
namespace Utils
{
	/**
	* \enum
	* \brief Masks for the possible errors that can happen during the initialization
	*/
	enum { NoError = 0, EmptyROMError = 1, ROMOverflowError = 2, GPUInitError = 4, 
		GPUWindowCreationError = 8, GPURendererError = 16, GPUTextureError = 32, 
		SPUError = 64, FileError = 128 };

	/**
	* \enum
	* \brief Masks for the possible errors that can happen during the emulation
	*/
	enum { ArithmeticError = 1, CallJumpError = 2, ConditionError = 4, MemoryError = 8, 
		DeviceError = 16, PaletteError = 32, StackError = 64, UnknownError = 128, EmulationDone = 256 };

	/**
	* \struct IfElse
	* \brief Defines a type depending on the condition by using partial specialization
	*/
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

	/**
	* \class Error
	* \brief Dummy class
	*/
	class Error;

	/**
	* \struct UInt_
	* \brief Defines an unsigned type that will at most contain the specified 
	*        number of bytes
	*/
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

	/**
	* \struct UInt_
	* \brief Defines an signed type that will at most contain the specified 
	*        number of bytes
	*/
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

	typedef UInt_<1>::type UInt8;	/**< Unsigned 8 bits integer type */
	typedef UInt_<2>::type UInt16;	/**< Unsigned 16 bits integer type */
	typedef UInt_<4>::type UInt32;	/**< Unsigned 32 bits integer type */

	typedef Int_<2>::type Int16;	/**< Signed 16 bits integer type */

	/**
	* \struct LeftShift
	* \brief Functor wrapping the left shift operator
	*/
	struct LeftShift
	{
		UInt16 operator()(const UInt16 & in_LHS, const UInt16 & in_RHS)
		{
			return in_LHS << in_RHS;
		}
	};

	/**
	* \struct ArithmeticRightShift
	* \brief Functor wrapping the right shift operator when the value to shift doesn't
	*        have its sign bit on. Otherwise, it uses a mask to copy the sign bit.
	*/
	struct ArithmeticRightShift
	{
		UInt16 operator()(const Int16 & in_LHS, const UInt16 & in_RHS)
		{
			// if(in_LHS < 0)
			//	return in_LHS >> in_RHS | ~(~0U >> in_RHS);
			//else
				return in_LHS >> in_RHS;
		}
	};

	/**
	* \struct LogicalRightShift
	* \brief Functor wrapping the right shift operator
	*/
	struct LogicalRightShift
	{
		UInt16 operator()(const UInt16 & in_LHS, const UInt16 & in_RHS)
		{
			return in_LHS >> in_RHS;
		}
	};
};

#endif // UTILS_H
