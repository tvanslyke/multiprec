#ifndef TIM_MULTIPREC_OPS_MULTIPLY_HPP
#define TIM_MULTIPREC_OPS_MULTIPLY_HPP

#include "tim/span/Span.hpp"
#include "tim/multiprec/ops/rshift.hpp"
#include "tim/multiprec/ops/ctz.hpp"
#include <array>
#include <optional>

namespace tim::multiprec::ops {

template <class Digit>
constexpr std::array<Digit, 2u> full_digit_multiply(Digit lhs, Digit rhs) {
	if constexpr(traits::digit_traits<Digit>::has_double_width_digit) {
		using wide_digit = typename traits::digit_traits<Digit>::double_width_digit_type;
		wide_digit res = static_cast<wide_digit>(lhs) * static_cast<wide_digit>(rhs);
		Digit lo = res & ~Digit(0);
		Digit hi = (res >> traits::digit_traits<Digit>::bits) & ~Digit(0);
		return {lo, hi};
	} else {
		constexpr std::size_t digit_bits = traits::digit_traits<Digit>::bits;
		constexpr std::size_t half_bits = digit_bits / 2u;
		constexpr std::size_t lo_mask = (~Digit(0)) >> half_bits;
		constexpr std::size_t hi_mask = ~(lo_mask);

		constexpr auto signed_diff = [](Digit l, Digit r) -> std::pair<Digit, bool> {
			if(l < r) {
				return std::make_pair(r - l, true);
			} else {
				return std::make_pair(l - r, false);
			}
		};

		const Digit lhs_lo = lhs & lo_mask;
		const Digit lhs_hi = (lhs & hi_mask) >> half_bits;
		const Digit rhs_lo = rhs & lo_mask;
		const Digit rhs_hi = (rhs & hi_mask) >> half_bits;

		// Karatsuba
		const Digit hi{lhs_hi * rhs_hi};
		const Digit lo{lhs_lo * rhs_lo};
		std::array<Digit, 2> result{(lo_mask & lo) << half_bits, (hi_mask & lo) >> half_bits};
		{
			bool carry = add_inplace(result, std::array<Digit, 2>{(lo_mask & hi) << half_bits, (hi_mask & hi) >> half_bits});
			assert(!carry);
		}
		{
			std::pair<Digit, bool> lhs_diff = signed_diff(lhs_lo, lhs_hi);
			std::pair<Digit, bool> rhs_diff = signed_diff(rhs_lo, rhs_hi);
			Digit mid = lhs_diff.first * rhs_diff.first;
			std::array<Digit, 2> tmp{(lo_mask & mid) << half_bits, (hi_mask & mid) >> half_bits};
			if(lhs_diff.second != rhs_diff.second) {
				bool borrow = subtract_inplace(result, tmp);
				assert(!borrow);
			} else {
				bool carry = add_inplace(result, tmp);
				assert(!carry);
			}
		}
		{
			bool carry = add_inplace(result, std::array<Digit, 2>{lo, hi});
			assert(!carry);
		}
		return result;
	}
}

template <class Digit>
constexpr bool multiply_add_single_digit(Span<Digit> out, Span<const Digit> lhs, Digit rhs) {
	bool carry = false;
	for(std::size_t i = 0; i < lhs.size(); ++i) {
		bool c = inplace_add(
			std::exchange(out, out.last(out.size() - 1)),
			full_digit_multiply(lhs[i], rhs)
		);
		if(c) {
			assert(!c || !carry);
		}
		carry = c;
	}
	return carry;
}

template <class Digit>
constexpr bool multiply_add_single_digit_backward(Span<Digit> out, Span<const Digit> lhs, Digit rhs) {
	bool carry = false;
	auto stop = out.rend();
	std::fill(out.begin() + lhs.size(), out.end(), Digit(0));
	for(std::size_t i = 0; i < lhs.size(); ++i) {
		bool carry = inplace_add(
			out.subspan(i, -1),
			full_digit_multiply(lhs[lhs.size() - (i + 1ul)], rhs)
		);
		if(c) {
			assert(!c || !carry);
		}
		carry = c;
	}
	return carry;
}

template <class Digit>
constexpr void multiply_polynomial(Span<Digit> out, Span<const Digit> lhs, Span<const Digit> rhs) {
	for(auto p = rhs.rbegin(); p < rhs.rend(); ++p) {
		bool multiply_add_single_digit_backward(
	}
}

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

#endif /* TIM_MULTIPREC_OPS_MULTIPLY_HPP */
