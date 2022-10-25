// The MIT License

#ifndef QR_CONTACT_SPEC_HPP
#define QR_CONTACT_SPEC_HPP

#include "common/qr_cTypes.h"

template <typename T>
class qrContactSpec {
public:

    /**
     * @brief qrContactSpec constructor
     * @param dim the dimension of contact space
     */
    qrContactSpec(size_t dim) : dim_contact_(dim), b_set_contact_(false) {
        idx_Fz_ = dim - 1;  // because normally (tau_x,y,z , linear_x,y,z)
        Fr_des_ = DVec<T>::Zero(dim);
    }

    virtual ~qrContactSpec() {}

    /**
     * @brief Get the dimension
     */
    size_t getDim() const { return dim_contact_; }
    
    /**
     * @brief 
     */
    size_t getDimRFConstraint() const { return Uf_.rows(); }
    
    /**
     * @brief 
     */
    size_t getFzIndex() const { return idx_Fz_; }

    /**
     * @brief 
     * @param Jc
     */
    void getContactJacobian(DMat<T>& Jc) { Jc = Jc_; }
    
    /**
     * @brief 
     * @param JcDotQdot
     */
    void getJcDotQdot(DVec<T>& JcDotQdot) { JcDotQdot = JcDotQdot_; }
    
    /**
     * @brief 
     */
    void UnsetContact() { b_set_contact_ = false; }

    /**
     * @brief 
     * @param Uf
     */
    void getRFConstraintMtx(DMat<T>& Uf) { Uf = Uf_; }
    
    /**
     * @brief 
     * @param ieq_vec
     */
    void getRFConstraintVec(DVec<T>& ieq_vec) { ieq_vec = ieq_vec_; }
    
    /**
     * @brief 
     */
    const DVec<T>& getRFDesired() { return Fr_des_; }
    
    /**
     * @brief 
     * @param Fr_des
     */
    void setRFDesired(const DVec<T>& Fr_des) { Fr_des_ = Fr_des; }

    /**
     * @brief 
     */
    bool UpdateqrContactSpec() {
        _UpdateJc();
        _UpdateJcDotQdot();
        _UpdateUf();
        _UpdateInequalityVector();
        b_set_contact_ = true;
        return true;
    }

protected:

    /**
     * @brief 
     */
    virtual bool _UpdateJc() = 0;
    
    /**
     * @brief 
     */
    virtual bool _UpdateJcDotQdot() = 0;
    
    /**
     * @brief 
     */
    virtual bool _UpdateUf() = 0;
    
    /**
     * @brief 
     */
    virtual bool _UpdateInequalityVector() = 0;

    int idx_Fz_;
    DMat<T> Uf_;
    DVec<T> ieq_vec_;
    DVec<T> Fr_des_;

    DMat<T> Jc_;
    DVec<T> JcDotQdot_;
    size_t dim_contact_;
    bool b_set_contact_;
};

#endif //QR_CONTACT_SPEC_HPP