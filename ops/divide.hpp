#ifndef TIM_MULTIPREC_OPS_DIVIDE_HPP
#define TIM_MULTIPREC_OPS_DIVIDE_HPP

#include "tim/span/Span.hpp"
#include "tim/multiprec/ops/rshift.hpp"
#include "tim/multiprec/ops/ctz.hpp"
#include <array>
#include <optional>

namespace tim::multiprec::ops {

template <class Digit>
constexpr std::optional<std::array<Digit, 2>> divide(std::array<Digit, 2> numer, Digit denom) {
	if(denom == 0ul) {
		return std::nullopt;
	}
	if(numer[1] == 0ul) {
		return std::array<Digit, 2>{numer[0] / denom, 0ul};
	}
	if(denom == 1ul) {
		return numer;
	}
	if(denom == 2ul) {
		inplace_rshift(numer);
		return numer;
	}
	
}

template <class Digit>
constexpr std::optional<std::array<Digit, 2>> divide_slowpath(std::array<Digit, 2> numer, Digit denom) {
	if constexpr(traits::digit_traits<Digit>::has_double_wide_digit) {
		using double_word = typename traits::digit_traits<Digit>::double_wide_digit_type;
		double_word n = (static_cast<double_word>(numer[1]) << traits::digit_traits<Digit>::bits) | numer[0];
		double_word d = denom;
		double_word quot = n / d;
		return std::array<Digit, 2>{
			static_cast<double_word>(quot & traits::digit_traits<Digit>::mask),
			static_cast<double_word>(quot >> traits::digit_traits<Digit>::bits)
		};
	}
}

}


} /* namespace tim::multiprec::ops */

#endif /* TIM_MULTIPREC_OPS_DIVIDE_HPP */
