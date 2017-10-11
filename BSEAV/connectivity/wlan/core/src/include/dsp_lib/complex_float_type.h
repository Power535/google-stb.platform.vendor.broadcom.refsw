#ifndef COMPLEX_CE_FLOAT_H_
#define COMPLEX_CE_FLOAT_H_

#include <iostream>

#include <dsp_lib/signed_mantissa.h>
#include <dsp_lib/signed_magnitude.h>
#include <dsp_lib/complex.h>
#include <dsp_lib/fifo.h>
// Undefine this if separate mantissa & exponent fifos not needed
// and you want to make debugging output much more concise when looking at an float_s variable
//#define SEP_FIFO



/*
 * class float_s - pseudo-floating
 * Template Arguments
 * - NMAN - number of bits allocated to the mantissa
 * Assumption:
 * 1. NMAN >= 2
 * 2. any operator between two complex_ce_float will have the same template parameters
 * - NEXP - number of bits allocated to the exponent
 */
template <typename MANT_TYPE, int32_t NMAN, int32_t NEXP> class complex_ce_float {

public:
	complex<MANT_TYPE> sm_mantissa;
	int32_t exponent;

	int log2int(double x) {
		double abs_x = fabs(x);
		return (int) floor((log(abs_x)/log((double) 2)));
	}


	complex_ce_float() {
		set_mantissas(0);
		set_mantissa_bits(NMAN);
		set_min_exponent();
	}
	complex_ce_float(float a) { to_complex_ce_float((double)a); }
	complex_ce_float(double a) { to_complex_ce_float(a); }
	complex_ce_float(int a) { to_complex_ce_float((double)a); }
    complex_ce_float(long a) { to_complex_ce_float((double)a); }


	void to_complex_ce_float(double x) {
		double man_dbl;
		set_mantissa_bits(NMAN);
		// Account for special case (log2 of 0 is inf)
		if(x == 0){
			set_min_exponent();
			set_mantissas(0);
		} else {
			set_exponent(log2int(x) - NMAN + 2);
			if(x>0) {
				man_dbl = x*pow(2.0,-((double)exponent)) + 0.5;
			} else {
				man_dbl = x*pow(2.0,-((double)exponent)) - 0.5;
			}
			set_mantissas(((int64_t)man_dbl));
		}
	}

	void setVal(double a) { to_complex_ce_float(a); }
	void setVal(float a)  { to_complex_ce_float((double)a); }
    void setVal(long a)   {	to_complex_ce_float((double)a); 	}
	void setVal(int a)    { to_complex_ce_float((double)a); 	}

	/* Get/Set Functions for Member Variables */
	int get_mantissa_bits() const { return(NMAN); }

	complex<int64_t> get_unsigned_mantissas() const {
		return(complex<int64_t>(sm_mantissa.i.get_unsigned_mantissa(),sm_mantissa.q.get_unsigned_mantissa()));}
	complex<int64_t> get_mantissas() const {
		return(complex<int64_t>(sm_mantissa.i.get_mantissa(),sm_mantissa.q.get_mantissa()));}
	complex<int64_t> get_signbits() const {
		return(complex<int64_t>(sm_mantissa.i.get_signbit(),sm_mantissa.q.get_signbit()));}
	complex<int64_t> get_mantissa_sm() const {
		return(complex<int64_t>(sm_mantissa.i.get_mantissa_sm(),sm_mantissa.q.get_mantissa_sm()));}

	int32_t get_exponent() const {return exponent;}

	void set_max_mantissas() {
		sm_mantissa.i.set_max_mantissa(); 
		sm_mantissa.q.set_max_mantissa(); 
	}
	void set_mantissa_bits(int x) {
		sm_mantissa.i.Bits = x; 
		sm_mantissa.q.Bits = x; 
	}
	void set_mantissas(int64_t x) {
		sm_mantissa.i.set_mantissa(x); 
		sm_mantissa.q.set_mantissa(0); 
	}
	void set_mantissas(complex<int64_t> x) {
		sm_mantissa.i.set_mantissa(x.real()); 
		sm_mantissa.q.set_mantissa(x.imag()); 
	}
	void force_unsigned_mantissa(uint64_t x) {
		sm_mantissa.i.force_unsigned_mantissa(x);
		sm_mantissa.q.set_mantissa(0);
	}
	void set_signbit_i(int x) {	sm_mantissa.i.set_signbit(x);	}
	void set_signbit_q(int x) {	sm_mantissa.q.set_signbit(x);	}
	void set_signbits(complex<int64_t> x) {	
		sm_mantissa.i.set_signbit((int)x.real());	
		sm_mantissa.q.set_signbit((int)x.imag());	
	}
	void set_exponent(int32_t x) {exponent = x;	}
	void set_min_exponent() {	exponent = -1 << (NEXP-1);}
	void set_max_exponent() {	exponent = (1 << (NEXP-1)) - 1;}

    void init_max() {
    	set_max_mantissas();
    	set_max_exponent();
    	set_signbits(0);
    }

	// Dummy align() function for complex block floating point.
	void align() {	;	}

	// EXPONENT ALIGNMENT: find max exponent
	int32_t findMaxExp(int32_t comExp) {
		if (get_exponent() > comExp) comExp = get_exponent();
		return comExp;
	}

	// EXPONENT ALIGNMENT: find min exponent
	int32_t findMinExp(int32_t comExp) {
		if (get_exponent() < comExp) comExp = get_exponent();
		return comExp;
	}

	// EXPONENT ALIGNMENT: Set common exponent
	int32_t setComExp(int32_t comExp) {
		if (get_exponent() < comExp) {
			sm_mantissa = sm_mantissa >> (comExp - get_exponent());
		} else {
			sm_mantissa = sm_mantissa << (get_exponent() - comExp);
		}
		set_exponent(comExp);
	}

	// Only operator & fifo stuff below here
	operator complex<double>() const {
		double ri = double(sm_mantissa.i.get_mantissa())*pow(2,double(exponent));
		double rq = double(sm_mantissa.q.get_mantissa())*pow(2,double(exponent));
		return(complex<double>(ri,rq));
	}

	// Re-normalize mantissas
	complex_ce_float alignednorm() {
		complex_ce_float tmp(*this);
		int li,lq,shift;
		li = sm_mantissa.i.get_shift_msbs();
		lq = sm_mantissa.q.get_shift_msbs();
		shift = MAX(li,lq);
		if (shift > 0) {
			tmp.sm_mantissa.i >>= shift;
			tmp.sm_mantissa.q >>= shift;
			tmp.exponent += shift;
		} else {
			tmp.sm_mantissa.i <<= -shift;
			tmp.sm_mantissa.q <<= -shift;
			tmp.exponent -= shift;
		}
		return(tmp);
	}


	// Left Shifting
	complex_ce_float<MANT_TYPE,NMAN, NEXP> operator <<(const int x) {
		complex_ce_float<MANT_TYPE, NMAN,NEXP> tmp;
		tmp.exponent = get_exponent() + x;
		tmp.sm_mantissa  = sm_mantissa;
		return tmp;
	};

	complex_ce_float<MANT_TYPE,NMAN, NEXP> operator <<=(const int x) {
		exponent = get_exponent() + x;
		return *this;
	};

	// Right shifting
	complex_ce_float<MANT_TYPE,NMAN, NEXP> operator >>(const int x) {
		complex_ce_float<MANT_TYPE,NMAN, NEXP> tmp;
		tmp.exponent = get_exponent() - x;
		tmp.sm_mantissa  = sm_mantissa;
		return tmp;
	};


	complex_ce_float<MANT_TYPE,NMAN, NEXP> operator >>=(const int x) {
		exponent = get_exponent() - x;
		return *this;
	};


    // various assignments from here below.
	template <typename MANT_TYPE1, int32_t NMAN1, int32_t EXPBITS1> 
		complex_ce_float<MANT_TYPE,NMAN,NEXP>& 
		operator =(complex_ce_float <MANT_TYPE1,NMAN1,EXPBITS1> &x) {
		if (NMAN > NMAN1) {
			sm_mantissa.i = x.sm_mantissa.i << (NMAN - NMAN1);
			sm_mantissa.q = x.sm_mantissa.q << (NMAN - NMAN1);
		} else {
			sm_mantissa.i = x.sm_mantissa.i >> (NMAN1 - NMAN);
			sm_mantissa.q = x.sm_mantissa.q >> (NMAN1 - NMAN);
		}
		exponent = x.exponent + NMAN1 - NMAN;
		set_mantissa_bits(NMAN);
				put_all_fifo();
		return *this;
	}
	/*
	complex_ce_float<MANT_TYPE,NMAN,NEXP>& 
		operator =(const complex_ce_float<MANT_TYPE,NMAN,NEXP> &x) {
		set_mantissas(x.get_mantissas());
		exponent = x.exponent;
		set_mantissa_bits(NMAN);
		//		put_all_fifo();
		return *this;
	}
	*/
	complex_ce_float& operator = (const double x) {
		to_complex_ce_float(x);
		set_mantissa_bits(NMAN);
				put_all_fifo();
		return *this;
	}

	complex_ce_float operator -() const {
		complex_ce_float tmp(*this);
		tmp.sm_mantissa.i.set_signbit(sm_mantissa.i.get_signbit() ^ 1);
		tmp.sm_mantissa.q.set_signbit(sm_mantissa.q.get_signbit() ^ 1);
		return(tmp);
	}

	complex_ce_float conj(void) {
		complex_ce_float tmp(*this);
		tmp.sm_mantissa.q.set_signbit(sm_mantissa.q.get_signbit() ^ 1);
		return(tmp);
	}
	//  floating point addition
	template <typename MANT_TYPE1, int32_t NMAN1, int32_t EXPBITS1> 
		complex_ce_float<MANT_TYPE, DMAX(NMAN, NMAN1) + 1, DMAX(NEXP, EXPBITS1)+1> 
		operator +(const complex_ce_float<MANT_TYPE1,NMAN1, EXPBITS1>& x) const {
		
		complex_ce_float<MANT_TYPE, DMAX(NMAN, NMAN1) + 1, DMAX(NEXP, EXPBITS1)+1> tmp;

		// If inputs aren't pre-aligned properly, the following routine aligns the operands
		int64_t exp_1 = exponent;
		int64_t exp_2 = x.exponent;
		int32_t man_shift;
		complex<MANT_TYPE> sman_1(sm_mantissa);
		complex<MANT_TYPE> sman_2(x.sm_mantissa);

		if(x.exponent > exponent) {
			sman_1 = sman_1  >> (x.exponent - exponent);
			exp_1 = exp_2;
			man_shift = (DMAX(NMAN, NMAN1) + 1) - (NMAN1 + 1);
		} else {
			if(exponent > x.exponent) {
				sman_2 = sman_2 >> (exponent - x.exponent);
				exp_2 = exp_1;
				man_shift = (DMAX(NMAN, NMAN1) + 1) - (NMAN + 1);
			} else {
				man_shift = 0;
			}
		}
		tmp.sm_mantissa = (sman_1 + sman_2);
		tmp.sm_mantissa = tmp.sm_mantissa << (man_shift);
		tmp.exponent = exp_2 - man_shift;
		return tmp;
	}
	// subtraction
	template <typename MANT_TYPE1, int32_t NMAN1, int32_t EXPBITS1> 
		complex_ce_float<MANT_TYPE, DMAX(NMAN, NMAN1) + 1, DMAX(NEXP, EXPBITS1)+1> 
		operator -(const complex_ce_float<MANT_TYPE1,NMAN1, EXPBITS1>& x) const {
		
		complex_ce_float<MANT_TYPE, DMAX(NMAN, NMAN1) + 1, DMAX(NEXP, EXPBITS1)+1> tmp;

		// If inputs aren't pre-aligned properly, the following routine aligns the operands
		int64_t exp_1 = exponent;
		int64_t exp_2 = x.exponent;
		int32_t man_shift;
		complex<MANT_TYPE> sman_1(sm_mantissa);
		complex<MANT_TYPE> sman_2(x.sm_mantissa);

		if(x.exponent > exponent) {
			sman_1 >>= (x.exponent - exponent);
			exp_1 = exp_2;
			man_shift = (DMAX(NMAN, NMAN1) + 1) - (NMAN1 + 1);
		} else {
			if(exponent > x.exponent) {
				sman_2 >>= (exponent - x.exponent);
				exp_2 = exp_1;
				man_shift = (DMAX(NMAN, NMAN1) + 1) - (NMAN + 1);
			} else {
				man_shift = 0;
			}
		}
		tmp.sm_mantissa = (sman_1 - sman_2);
		tmp.sm_mantissa = tmp.sm_mantissa << (man_shift);
		tmp.exponent = exp_2 - man_shift;
		return tmp;
	}


	// various accumulators, i.e., += or -= from here below.
	template <typename MANT_TYPE1, int32_t NMAN1, int32_t EXPBITS1> 
		complex_ce_float<MANT_TYPE,NMAN,NEXP>& 
		operator +=(const complex_ce_float<MANT_TYPE1,NMAN1,EXPBITS1>& x) {
		int64_t exp_1 = exponent;
		int64_t exp_2 = x.exponent;

		complex<MANT_TYPE> sman_1(sm_mantissa); 
		complex<MANT_TYPE> sman_2(x.sm_mantissa);
		
		if(NMAN > NMAN1) {
			sman_2 <<= (NMAN-NMAN1);
			exp_2  -= (NMAN-NMAN1);
		} else {
			sman_2 >>= (NMAN1-NMAN);
			exp_2  += (NMAN1-NMAN);
		}

		if ((exp_1 - x.exponent) >= NMAN ) {
			sm_mantissa = sman_1;
		} else {
			if(exp_2 > exp_1) {
				sman_1 >>= (exp_2 - exp_1);
				exp_1 = exp_2;
			} else {
				if(exp_1 > exp_2) {
					sman_2 >>= (exp_1 - exp_2);
					exp_2 = exp_1;
				} else {
				}
			}
			sm_mantissa = (sman_1 + sman_2);
			if ( (x.exponent - exp_1) > NMAN ) {
				set_signbits(x.get_signbits());
			}
		}
		exponent=exp_2;
		put_all_fifo();
		return *this;
	}
	template <typename MANT_TYPE1, int32_t NMAN1, int32_t EXPBITS1> complex_ce_float<MANT_TYPE,NMAN,NEXP> & 
		operator -=(const complex_ce_float<MANT_TYPE,NMAN1,EXPBITS1>& x) {
		int64_t exp_1 = exponent;
		int64_t exp_2 = x.exponent;
		complex<MANT_TYPE> sman_1(sm_mantissa);  
		complex<MANT_TYPE> sman_2(x.sm_mantissa);
		if(exp_2 > exponent) {
			sman_1 >>= (exp_2 - exponent);
			exp_1 = exp_2;
		} else {
			if(exponent > exp_2) {
				sman_2 >>= (exponent - exp_2);
				exp_2 = exponent;
			} else {
			}
		}
		sm_mantissa = (sman_1 - sman_2);
		exponent = exp_2;
				put_all_fifo();
		return *this;
	}

	// Casting operator
	template <typename MANT_TYPE1, int32_t NMAN1, int32_t EXPBITS1> 
		operator complex_ce_float<MANT_TYPE,NMAN1,EXPBITS1>() const {
		complex_ce_float<MANT_TYPE,NMAN1,EXPBITS1> tmp;
		int32_t manwidth1 = NMAN; int32_t manwidth2 = NMAN1;
		if (NMAN > NMAN1) {
			tmp.sm_mantissa = sm_mantissa >> (NMAN-NMAN1);
			tmp.exponent = exponent + (NMAN-NMAN1);
		} else {
			tmp.sm_mantissa = sm_mantissa << (NMAN1-NMAN);
			tmp.exponent = exponent - (NMAN1-NMAN);
		}
		return tmp;
	}

	uint64_t pack() {
		int ebits = exponent & ( (1<<NEXP) - 1);
		set_mantissa_bits(NMAN);
		uint64_t x = ((sm_mantissa.i.get_mantissa_sm() << (NEXP+NMAN)) + 
					  (sm_mantissa.q.get_mantissa_sm() << NEXP) + ebits);
		return(x);
	}
	void unpack(uint64_t x) {
		int ex = x & ( (1<<NEXP) - 1);
		// check for negaitve exponent
		if (ex & (1 << (NEXP-1))) {
			ex -= (1 << NEXP);
		}
		int i_sign = ( x >> (NEXP+NMAN-1) ) & 1;
		int q_sign = ( x >> (NEXP+2*NMAN-1) ) & 1;
		uint64_t i_man  = ( x >> (NEXP+NMAN) ) & ((1<<(NMAN-1)) - 1);
		uint64_t q_man  = ( x >> (NEXP) ) & ((1<<(NMAN-1)) - 1);
		
		set_exponent(ex);
		sm_mantissa.i.set_signbit(i_sign);
		sm_mantissa.q.set_signbit(q_sign);
		sm_mantissa.i.set_unsigned_mantissa(i_man);
		sm_mantissa.q.set_unsigned_mantissa(q_man);
	}



	/*
	// Boolean operators for floating type numbers with same precision
	bool operator ==(const complex_ce_float& x) const { return ((get_mantissa() == x.get_mantissa())&&(exponent == x.get_exponent())&&(get_signbit() == x.get_signbit())); }
	bool operator !=(const complex_ce_float& x) const { return ((get_mantissa() != x.get_mantissa())||(exponent != x.get_exponent())||(get_signbit() != x.get_signbit())); }
	bool operator  <(const complex_ce_float& x) const { return ((to_double() < x.to_double())); }
	bool operator  >(const complex_ce_float& x) const { return ((to_double() > x.to_double())); }
	bool operator <=(const complex_ce_float& x) const { return ((to_double() <= x.to_double())); }
	bool operator >=(const complex_ce_float& x) const { return ((to_double() >= x.to_double())); }

	// Boolean operators for floating type numbers with different precisions
	template <typename MANT_TYPE, int32_t NMAN1, int32_t EXPBITS1> bool operator ==(const complex_ce_float<MANT_TYPE,NMAN1, EXPBITS1>& x) const {
		return ((to_double() == x.to_double()));
	}
	template <typename MANT_TYPE, int32_t NMAN1, int32_t EXPBITS1> bool operator !=(const complex_ce_float<MANT_TYPE,NMAN1, EXPBITS1>& x) const {
		return ((to_double() != x.to_double()));
	}
	template <typename MANT_TYPE, int32_t NMAN1, int32_t EXPBITS1> bool operator <(const complex_ce_float<MANT_TYPE,NMAN1, EXPBITS1>& x) const {
		return ((to_double() < x.to_double()));
	}
	template <typename MANT_TYPE, int32_t NMAN1, int32_t EXPBITS1> bool operator <=(const complex_ce_float<MANT_TYPE,NMAN1, EXPBITS1>& x) const {
		return ((to_double() <= x.to_double()));
	}
	template <typename MANT_TYPE, int32_t NMAN1, int32_t EXPBITS1> bool operator >(const complex_ce_float<MANT_TYPE,NMAN1, EXPBITS1>& x) const {
		return ((to_double() > x.to_double()));
	}
	template <typename MANT_TYPE, int32_t NMAN1, int32_t EXPBITS1> bool operator >=(const complex_ce_float<MANT_TYPE,NMAN1, EXPBITS1>& x) const {
		return ((to_double() >= x.to_double()));
	}
	*/
#ifndef COSIM_ASSIGNMENT_PUT
	void put_all_fifo(void) {;}
#else
	void put_all_fifo(void) {
		put_fifo_data();
		// these do nothing unless SEP_FIFO is defined
		put_fifo_mantissa();
		put_fifo_exponent();
	}
#endif

	// COSIM & fifo & I/O stuff below til' end of class def
#ifdef COSIM
	fifo<long>	fifo_data;
#ifdef SEP_FIFO
	fifo<int>	fifo_mantissa;
	fifo<int>	fifo_exponent;
#endif

	void init_fifo_all(int len) {
		// setting fifo memory depth.
		fifo_data.set_size(len);
		// these do nothing unless SEP_FIFO is defined
		fifo_mantissa.set_size(len);
		fifo_exponent.set_size(len);
	}

	void init_fifo(int len) {fifo_data.set_size(len);}
	void init_fifo_data(int len) {fifo_data.set_size(len);}
	long get_fifo_data() { return(fifo_data.get_data());	}
	void put_fifo_data() {
		int ebits = exponent & ( (1<<NEXP) - 1);
		set_mantissa_bits(NMAN);
		fifo_data.put((get_mantissa_sm() << NEXP) + ebits);
	}
#ifdef SEP_FIFO
	int get_fifo_mantissa() {		return(fifo_mantissa.get_data());	}
	int get_fifo_exponent() {		return(fifo_exponent.get_data());	}
#endif

	void init_fifo_mantissa(int len) {
#ifdef SEP_FIFO
		fifo_mantissa.set_size(len);
#endif
	}
	void init_fifo_exponent(int len) {
#ifdef SEP_FIFO
		fifo_exponent.set_size(len);
#endif
	}
	void put_fifo_mantissa() {	
#ifdef SEP_FIFO
		fifo_mantissa.put(get_mantissa_sm());	
#endif
	}
	void put_fifo_exponent() {
#ifdef SEP_FIFO
		int ebits = exponent & ( (1<<NEXP) - 1);
		fifo_exponent.put(ebits);
#endif
	}
#endif

	// IO functions
	friend std::ostream& 
		operator <<(std::ostream& os, complex_ce_float<MANT_TYPE,NMAN, NEXP> &r) {
		// Convert to complex double 1st!!!
		complex<double> d = complex<double>(r);
		return os << d;
	}
	friend std::istream& 
		operator >>(std::istream& os, complex_ce_float<MANT_TYPE,NMAN, NEXP> &r) {
		complex<double> rtmp;
		std::istream tmp = os >> rtmp;
		r = rtmp;
		return tmp;
	}

	// IO functions
	friend std::ostream& 
		operator <<(std::ostream& os, const complex_ce_float<MANT_TYPE,NMAN, NEXP> &r) {
		// Convert to complex double 1st!!!
		complex<double> d = complex<double>(r);
		return os << d;
	}

}; // END OF CLASS COMPLEX_CE_FLOAT


template <typename MANT_TYPE, int32_t NMAN, int32_t NEXP> 
	complex_ce_float<MANT_TYPE,NMAN,NEXP> conj(complex_ce_float<MANT_TYPE,NMAN,NEXP>& x) {
	complex_ce_float<MANT_TYPE,NMAN,NEXP> tmp(x);
	tmp.sm_mantissa.q.set_signbit(x.sm_mantissa.q.get_signbit() ^ 1);
	return(tmp);
}

// NEED TO SPECIALIZE MANTISSA SIZE DEPENDING ON TYPE!
template <typename MANT_TYPE1, int32_t NMAN1, int32_t EXPBITS1,
	typename MANT_TYPE2, int32_t NMAN2, int32_t EXPBITS2> 
	complex_ce_float<MANT_TYPE1,NMAN1+NMAN2-1,DMAX(EXPBITS2,EXPBITS1)+1> 
	operator *(const complex_ce_float<MANT_TYPE1,NMAN1, EXPBITS1>& r,
			   const complex_ce_float<MANT_TYPE2,NMAN2, EXPBITS2>& l) 
{
	complex_ce_float<MANT_TYPE1, NMAN1+NMAN2-1,DMAX(EXPBITS2,EXPBITS1)+1> tmp;
	tmp.exponent = r.exponent + l.exponent;
	// Complex MULT here -.. for now use this...
	tmp.sm_mantissa = r.sm_mantissa * l.sm_mantissa;
	return tmp;
}

// Re-normalize mantissas
template <typename MANT_TYPE1, int32_t NMAN1, int32_t EXPBITS1>
	complex_ce_float<MANT_TYPE1,NMAN1,EXPBITS1> 
	alignednorm(const complex_ce_float<MANT_TYPE1,NMAN1, EXPBITS1>& l) {
	complex_ce_float<MANT_TYPE1,NMAN1,EXPBITS1> tmp(l);
	int li,lq,shift;
	
	li = tmp.sm_mantissa.i.get_shift_msbs();
	lq = tmp.sm_mantissa.q.get_shift_msbs();
	shift = MAX(li,lq);
	// Shift should always be <= 0 unless somehow mantissa has grown beyond it's correct size
	if (shift > 0) {
		tmp.sm_mantissa.i >>= shift;
		tmp.sm_mantissa.q >>= shift;
	} else {
		tmp.sm_mantissa.i <<= -shift;
		tmp.sm_mantissa.q <<= -shift;
	}
	tmp.exponent += shift;
	return(tmp);
}



#endif /*COMPLEX_CE_FLOAT_H_*/
