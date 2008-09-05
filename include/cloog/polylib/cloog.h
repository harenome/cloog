#ifndef CLOOG_POLYLIB_H
#define CLOOG_POLYLIB_H

#define CLOOG_POLYLIB
#include <cloog/cloog.h>


/******************************************************************************
 *                             CLOOG GMP MACROS                               *
 ******************************************************************************/
#ifdef GNUMP
#define value_init_c(val)              { mpz_init((val)) ; \
                                         cloog_int_leak_up() ; \
                                       }
#define value_clear_c(val)             { mpz_clear((val)) ; \
                                         cloog_int_leak_down() ; \
                                       }
#define value_sprint(Dst,fmt,val)      { char * str ; \
                                         str = mpz_get_str(0,10,(val)) ; \
                                         sprintf((Dst),(fmt),str) ; \
                                         free(str) ; \
                                       }
#define value_eq_si(v1,i2)             (mpz_cmp_si((v1),(i2)) == 0)
#define value_ne_si(v1,i2)             (mpz_cmp_si((v1),(i2)) != 0)
#define value_gt_si(v1,i2)             (mpz_cmp_si((v1),(i2)) > 0)
#define value_lt_si(v1,i2)             (mpz_cmp_si((v1),(i2)) < 0)
#define value_multiply_si(ref,v1,i2)   (mpz_mul_si((ref),(v1),(i2)))


/******************************************************************************
 *                          CLOOG BASIC TYPES MACROS                          *
 ******************************************************************************/
#else
#define value_init_c(val)              ((val) = 0)
#define value_clear_c(val)             ((val) = 0)
#define value_sprint(Dst,fmt,val)      (sprintf((Dst),(fmt),(val)))
#define value_eq_si(v1,i2)             ((v1) == (i2))
#define value_ne_si(v1,i2)             ((v1) != (i2))
#define value_gt_si(v1,i2)             ((v1) > (i2))
#define value_lt_si(v1,i2)             ((v1) < (i2))
#define value_multiply_si(ref,v1,i2)   ((ref) = value_mult((v1),(i2)))
#endif

#ifndef value_subtract
#define value_subtract value_substract
#endif


/******************************************************************************
 *                             POLYLIB MACROS                                 *
 ******************************************************************************
 * Because I was tired to look in polylib/include/polylib/arithmetique.h for  *
 * the existing macros, here is a copy !                                      *
 ******************************************************************************/
#if 0

/******************************************************************************
 *                           POLYLIB GMP MACROS                               *
 ******************************************************************************/
#if defined(GNUMP)

/* Basic macros */

#define value_init(val)                (mpz_init((val)))
#define value_assign(v1,v2)            (mpz_set((v1),(v2)))
#define value_set_si(val,i)            (mpz_set_si((val),(i)))    
#define value_set_double(val,d)        (mpz_set_d((val),(d)))
#define value_clear(val)               (mpz_clear((val)))
#define value_read(val,str)            (mpz_set_str((val),(str),10))
#define value_print(Dst,fmt,val)       {char *str; \
                                        str = mpz_get_str(0,10,(val)); \
                                        fprintf((Dst),(fmt),str); free(str); \
                                       }
#define value_swap(val1,val2)          {mpz_t tmp; mpz_init_set(tmp,(val1)); \
                                        mpz_set((val1),(val2)); \
                                        mpz_set((val2),tmp); \
                                        mpz_clear(tmp); \
                                       }
                                             
/* Boolean operators on 'Value' */

#define value_eq(v1,v2)                (mpz_cmp((v1),(v2)) == 0)
#define value_ne(v1,v2)                (mpz_cmp((v1),(v2)) != 0)
#define value_gt(v1,v2)                (mpz_cmp((v1),(v2))  > 0)
#define value_ge(v1,v2)                (mpz_cmp((v1),(v2)) >= 0)
#define value_lt(v1,v2)                (mpz_cmp((v1),(v2))  < 0)
#define value_le(v1,v2)                (mpz_cmp((v1),(v2)) <= 0)

/* Trian operators on 'Value' */

#define value_sign(val)                (mpz_sgn(val))
#define value_compare(v1,v2)           (mpz_cmp((v1),(v2)))

/* Binary operations on 'Value' */

#define value_addto(ref,val1,val2)     (mpz_add((ref),(val1),(val2)))
#define value_add_int(ref,val,vint)    (mpz_add_ui((ref),(val),(long)(vint)))
#define value_increment(ref,val)       (mpz_add_ui((ref),(val),1))
#define value_multiply(ref,val1,val2)  (mpz_mul((ref),(val1),(val2)))
#define value_substract(ref,val1,val2) (mpz_sub((ref),(val1),(val2)))
#define value_sub_int(ref,val,vint)    (mpz_sub_ui((ref),(val),(long)(vint)))
#define value_decrement(ref,val)       (mpz_sub_ui((ref),(val),1))
#define value_division(ref,val1,val2)  (mpz_tdiv_q((ref),(val1),(val2)))
#define value_modulus(ref,val1,val2)   (mpz_tdiv_r((ref),(val1),(val2)))
#define value_pdivision(ref,val1,val2) (mpz_fdiv_q((ref),(val1),(val2)))
#define value_oppose(ref,val)          (mpz_neg((ref),(val)))
#define value_absolute(ref,val)        (mpz_abs((ref),(val)))
#define value_minimum(ref,val1,val2)   (value_le((val1),(val2)) ? \
                                        mpz_set((ref),(val1)) : \
                                        mpz_set((ref),(val2)))  
#define value_maximum(ref,val1,val2)   (value_ge((val1),(val2)) ? \
                                        mpz_set((ref),(val1)) : \
                                        mpz_set((ref),(val2)))  
#define value_orto(ref,val1,val2)      (mpz_ior((ref),(val1),(val2)))
#define value_andto(ref,val1,val2)     (mpz_and((ref),(val1),(val2)))

/* Conditional operations on 'Value' */

#define value_pos_p(val)               (mpz_sgn(val) >  0)
#define value_neg_p(val)               (mpz_sgn(val) <  0)
#define value_posz_p(val)              (mpz_sgn(val) >= 0)
#define value_negz_p(val)              (mpz_sgn(val) <= 0)
#define value_zero_p(val)              (mpz_sgn(val) == 0)
#define value_notzero_p(val)           (mpz_sgn(val) != 0)
#define value_one_p(val)               (mpz_cmp_si(val,1) == 0)
#define value_notone_p(val)            (mpz_cmp_si(val,1) != 0)
#define value_mone_p(val)              (mpz_cmp_si(val,-1) ==0)
#define value_notmone_p(val)           (mpz_cmp_si(val,-1) !=0)


/******************************************************************************
 *                        POLYLIB BASIC TYPES MACROS                          *
 ******************************************************************************/
#else /* 'Value' set to longlong|long|float|char *|int */                                     	

/* Basic Macros */    				    

#define value_init(val)                ((val) = 0)
#define value_assign(v1,v2)            ((v1)  = (v2))
#define value_set_si(val,i)            ((val) = (Value)(i))   
#define value_set_double(val,d)        ((val) = (Value)(d)) 
#define value_clear(val)               ((val) = 0)
#define value_read(val,str)            (sscanf((str),VALUE_FMT,&(val)))
#define value_print(Dst,fmt,val)       (fprintf((Dst),(fmt),(val)))
#define value_swap(v1,v2)              {Value tmp; tmp = v2; \
                                        v2 = v1; v1 = tmp; \
                                       }
/* Cast to 'Value' */

#define int_to_value(i)                ((Value)(i))
#define long_to_value(l)               ((Value)(l))
#define float_to_value(f)              ((Value)(f))
#define double_to_value(d)             ((Value)(d))
   
/* Boolean operators on 'Value' */

#define value_eq(v1,v2)                ((v1)==(v2))
#define value_ne(v1,v2)                ((v1)!=(v2))
#define value_gt(v1,v2)                ((v1)>(v2))
#define value_ge(v1,v2)                ((v1)>=(v2))
#define value_lt(v1,v2)                ((v1)<(v2))
#define value_le(v1,v2)                ((v1)<=(v2))

/* Trian operators on 'Value' */

#define value_sign(v)                  (value_eq(v,VALUE_ZERO) ? \
                                        0: \
                                        value_lt(v,VALUE_ZERO)?-1:1)
#define value_compare(v1,v2)           (value_eq(v1,v2)? \
                                        0: \
                                        value_lt(v1,v2)?-1:1)

/* Binary operators on 'Value' */

#define value_plus(v1,v2)  		((v1)+(v2))
#define value_div(v1,v2)   		((v1)/(v2))
#define value_mod(v1,v2)   		((v1)%(v2))
#define value_direct_multiply(v1,v2)	((v1)*(v2)) /* direct! */
#define value_minus(v1,v2) 		((v1)-(v2))
#define value_pdiv(v1,v2)  		(divide((v1),(v2)))
#define value_pmod(v1,v2)  		(modulo((v1),(v2)))
#define value_min(v1,v2)   		(value_le((v1),(v2))? (v1): (v2))
#define value_max(v1,v2)   		(value_ge((v1),(v2))? (v1): (v2))
#define value_or(v1,v2)  		((v1)|(v2))
#define value_and(v1,v2)  		((v1)&(v2))
#define value_lshift(v1,v2)     	((v1)<<(v2))
#define value_rshift(v1,v2)  	        ((v1)>>(v2))
				  
/* Binary operations on 'Value' */ 

#define value_addto(ref,val1,val2) 	((ref) = (val1)+(val2))
#define value_add_int(ref,val,vint)     ((ref) = (val)+(Value)(vint))
#define value_increment(ref,val) 	((ref) = (val)+VALUE_ONE)
#define value_direct_product(ref,val1,val2) ((ref) = (val1)*(val2)) /* direct */
#define value_multiply(ref,val1,val2)	((ref) = value_mult((val1),(val2)))
#define value_substract(ref,val1,val2) 	((ref) = (val1)-(val2))
#define value_sub_int(ref,val,vint)     ((ref) = (val)-(Value)(vint))
#define value_decrement(ref,val) 	((ref) = (val)-VALUE_ONE)
#define value_division(ref,val1,val2) 	((ref) = (val1)/(val2))
#define value_modulus(ref,val1,val2) 	((ref) = (val1)%(val2))
#define value_pdivision(ref,val1,val2)	((ref) = value_pdiv((val1),(val2)))
#define value_oppose(ref,val)    	((ref) = value_uminus((val)))
#define value_absolute(ref,val)		((ref) = value_abs((val)))
#define value_minimum(ref,val1,val2)	((ref) = value_min((val1),(val2)))
#define value_maximum(ref,val1,val2)	((ref) = value_max((val1),(val2)))
#define value_orto(ref,val1,val2)	((ref) = (val1)|(val2))
#define value_andto(ref,val1,val2)	((ref) = (val1)&(val2))

/* Unary operators on 'Value' */

#define value_uminus(val)               (-(val))
#define value_not(val)	                (~(val))
#define value_abs(val)                  (value_posz_p(val)? \
                                         (val) : \
                                         (value_ne((val), VALUE_NAN) ? \
                                          value_uminus(val) : \
                                          (THROW (overflow_error), VALUE_NAN )))

/* Conditional operations on 'Value' */

#define value_pos_p(val)                value_gt(val,VALUE_ZERO)
#define value_neg_p(val)                value_lt(val,VALUE_ZERO)
#define value_posz_p(val)               value_ge(val,VALUE_ZERO)
#define value_negz_p(val)               value_le(val,VALUE_ZERO)
#define value_zero_p(val)               value_eq(val,VALUE_ZERO)
#define value_notzero_p(val)            value_ne(val,VALUE_ZERO)
#define value_one_p(val)                value_eq(val,VALUE_ONE)
#define value_notone_p(val)             value_ne(val,VALUE_ONE)
#define value_mone_p(val)               value_eq(val,VALUE_MONE)
#define value_notmone_p(val)            value_ne(val,VALUE_MONE)
#define value_min_p(val)                value_eq(val,VALUE_MIN)
#define value_max_p(val)                value_eq(val,VALUE_MAX)
#define value_notmin_p(val)             value_ne(val,VALUE_MIN)
#define value_notmax_p(val)             value_ne(val,VALUE_MAX)

#endif /* 'Value' set to |longlong|long|float|char *|int */
#endif

#endif /* define _H */
