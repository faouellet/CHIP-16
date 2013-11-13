//#ifndef GLOBAL_ALLOCATOR_H
//#define GLOBAL_ALLOCATOR_H
//
//#include "utils.h"
//
//#include <vector>
//
///**
//* \class GlobalAllocator
//* \brief TODO : Would be used for inter-block analysis and allocation
//*/
//class GlobalAllocator
//{
//private:
//	std::vector<std::vector<std::pair<Utils::UInt16, Utils::UInt16>>> m_LiveIntervals;
//
//private:
//	/**
//	* \fn AnalyzeRegisterLiveness
//	* \brief TODO
//	* TODO : Add ref to Dragon book
//	*/
//	void AnalyzeRegisterLiveness();
//
//public:
//	/**
//	* \fn GetAvailableRegister
//	* \brief Find an available register in the register pool. If it fails, it will make a
//	*        register available by spilling it to memory
//	*/
//	Utils::UInt8 GetAvailableRegister();
//};
//
//#endif // GLOBAL_ALLOCATOR_H
