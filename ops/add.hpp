#ifndef TIM_MULTIPREC_OPS_ADD_HPP
#define TIM_MULTIPREC_OPS_ADD_HPP

#include "tim/span/Span.hpp"
#include "tim/multiprec/ops/rshift.hpp"
#include "tim/multiprec/ops/ctz.hpp"
#include <array>
#include <optional>

namespace tim::multiprec::ops {

template <class Digit>
constexpr Digit add_with_carry(Digit lhs, Digit rhs, bool& carry) {
	Digit result = lhs + rhs;
	if((result < lhs) || (result < rhs)) {
		++result;
		carry = true;
	} else if(carry) {
		++result;
		carry = (result == Digit(0u));
	} else {
		carry = false;
	}
	return std::make_pair(result, carry);
}

template <class Digit>
constexpr bool add(Span<const Digit> lhs, Span<const Digit> rhs, Span<Digit> out) {
	if(lhs.size() > rhs.size()) {
		std::swap(lhs, rhs);
	}
	bool carry = false;
	std::size_t index = 0;
	for(; index < lhs.size(); ++i) {
		out[index] = add_with_carry(lhs[index], rhs[index], carry);
	}
	if(carry) {
		for(; index < rhs.size(); ++index) {
			Digit incr = rhs[index];
			++incr;
			out[index] = incr;
			if(incr != Digit(0u)) {
				carry = false;
				break;
			}
		}
	}
	std::copy(rhs.begin() + index, rhs.end(), out.begin() + index);
	return carry;
}

} /* namespace tim::multiprec::ops */

#endif /* TIM_MULTIPREC_OPS_ADD_HPP */
