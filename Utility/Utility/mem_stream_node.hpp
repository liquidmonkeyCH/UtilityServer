/**
* @file mem_stream_node.hpp
*
* @author Hourui (liquidmonkey)
*/
#ifndef __MEM_STREAM_NODE_HPP__
#define __MEM_STREAM_NODE_HPP__

#include "base_defines.hpp"

namespace Utility
{
////////////////////////////////////////////////////////////////////////////////////////////////////
namespace mem
{
////////////////////////////////////////////////////////////////////////////////////////////////////
template<std::size_t N>
struct stream_node
{
	static constexpr std::size_t MAX_LEN = N;
	stream_node(void) :m_next(nullptr) { m_buffer[N] = 0; }
	~stream_node(void) = default;

	char m_buffer[N + 1];
	stream_node* m_next;
};
////////////////////////////////////////////////////////////////////////////////////////////////////
}// namespace mem
////////////////////////////////////////////////////////////////////////////////////////////////////
}//namespace Utility 
#endif //__MEM_BUFFER_HPP__