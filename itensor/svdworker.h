#ifndef __ITENSOR_SVDWORKER_H
#define __ITENSOR_SVDWORKER_H
#include "iqcombiner.h"
#include "iqtsparse.h"

enum Direction { Fromright, Fromleft, Both, None };

template<class Tensor, class IndexT>
IndexT
index_in_common(const Tensor& A, const Tensor& B, IndexType t)
    {
    for(int j = 1; j <= A.r(); ++j)
        {
        const IndexT& I = A.index(j);
        if(I.type() == t && B.hasindex(I)) { return I; }
        }
    return IndexT();
    }

Index inline
index_in_common(const ITensor& A, const ITensor& B, IndexType t)
    { return index_in_common<ITensor,Index>(A,B,t); }

IQIndex inline
index_in_common(const IQTensor& A, const IQTensor& B, IndexType t)
    { return index_in_common<IQTensor,IQIndex>(A,B,t); }

//
// SVDWorker
//

class SVDWorker
    {
    public:

    //
    // Constructors
    //

    SVDWorker();

    SVDWorker(int N_);

    SVDWorker(int N_, Real cutoff, int minm, int maxm, 
              bool doRelCutoff, const LogNumber& refNorm);

    SVDWorker(std::istream& s) { read(s); }

    //
    // Site Canonical SVD 
    // (weights in both L and R, inverse weights in V)
    //

    template <class Tensor,class SparseT> 
    void 
    csvd(const Tensor& AA, Tensor& L, SparseT& V, Tensor& R)
        { 
        csvd<Tensor>(1,AA,L,V,R); 
        }

    template <class Tensor, class SparseT> 
    void 
    csvd(int b, const Tensor& AA, Tensor& L, SparseT& V, Tensor& R);

    //Object PH of type LocalOpT has to provide the deltaRho method
    //to use the noise term feature (see projectedop.h)
    template <class Tensor, class SparseT, class LocalOpT>
    void 
    csvd(int b, const LocalOpT& PH, 
          const Tensor& AA, Tensor& L, SparseT& V, Tensor& R);


    //
    // Density Matrix Decomposition
    // (weights in B for dir == Fromleft, weights in A for dir == Fromright)
    //

    template <class Tensor> 
    void 
    denmatDecomp(const Tensor& AA, Tensor& A, Tensor& B, Direction dir)
        { 
        denmatDecomp<Tensor>(1,AA,A,B,dir); 
        }

    template <class Tensor> 
    void
    denmatDecomp(int b, const Tensor& AA, Tensor& A, Tensor& B, Direction dir);

    //Object PH of type LocalOpT has to provide the deltaRho method
    //to use the noise term feature (see projectedop.h)
    template <class Tensor, class LocalOpT>
    void
    denmatDecomp(int b, const LocalOpT& PH, 
         const Tensor& AA, Tensor& A, Tensor& B, Direction dir);

    //
    // Singular Value Decomposition
    // (a.k.a. bond canonical SVD; weights in D)
    //

    template <class Tensor, class SparseT> 
    void 
    svd(const Tensor& AA, Tensor& U, SparseT& D, Tensor& V)
        { 
        svd<Tensor>(1,AA,U,D,V); 
        }

    template <class Tensor, class SparseT> 
    void 
    svd(int b, const Tensor& AA, Tensor& U, SparseT& D, Tensor& V);

    //Object PH of type LocalOpT has to provide the deltaRho method
    //to use the noise term feature (see projectedop.h)
    template <class Tensor, class SparseT, class LocalOpT>
    void 
    svd(int b, const LocalOpT& PH, 
        const Tensor& AA, Tensor& U, SparseT& D, Tensor& V);


    //
    // Accessor Methods
    //

    int 
    NN() const { return N; }

    Real 
    cutoff() const { return cutoff_; }
    void 
    cutoff(Real val) { cutoff_ = val; }

    Real 
    truncerr(int b = 1) const { return truncerr_.at(b); }

    int 
    minm() const { return minm_; }
    void 
    minm(int val) { minm_ = val; }

    int 
    maxm() const { return maxm_; }
    void 
    maxm(int val) { maxm_ = val; }

    bool 
    useOrigM() const { return use_orig_m_; }
    void 
    useOrigM(bool val) { use_orig_m_ = val; }

    bool 
    showeigs() const { return showeigs_; }
    void 
    showeigs(bool val) { showeigs_ = val; }

    // If doRelCutoff_ is false,
    // refNorm_ defines an overall scale factored
    // out of the denmat before truncating.
    //
    // If doRelCutoff_ is true, refNorm_
    // is determined automatically.
    //
    bool 
    doRelCutoff() const { return doRelCutoff_; }
    void 
    doRelCutoff(bool val) { doRelCutoff_ = val; }

    // If absoluteCutoff_ == true, do a strict limit
    // on the eigenvalues of the density matrix, 
    // not trying for a truncation error
    bool 
    absoluteCutoff() const { return absoluteCutoff_; }
    void 
    absoluteCutoff(bool val) { absoluteCutoff_ = val; }

    LogNumber 
    refNorm() const { return refNorm_; }
    void 
    refNorm(const LogNumber& val) 
        { 
        if(val.sign() == 0) Error("zero refNorm");
        refNorm_ = val; 
        }

    const Vector& 
    eigsKept(int b = 1) const 
        { return eigsKept_.at(b); }
    int
    numEigsKept(int b = 1) const 
        { return eigsKept_.at(b).Length(); }

    int 
    maxEigsKept() const;

    Real 
    maxTruncerr() const;

    Real
    noise() const { return noise_; }
    void
    noise(Real val) { noise_ = val; }

    //
    // Other Methods
    //

    Real 
    diag_denmat(const ITensor& rho, Vector& D, Index& newmid, ITensor& U);
    Real 
    diag_denmat(const IQTensor& rho, Vector& D, IQIndex& newmid, IQTensor& U);

    Real 
    diag_denmat(const ITensor& rho, Vector& D, Index& newmid, 
                ITensor& C, ITensor& U);
    Real 
    diag_denmat(const IQTensor& rho, Vector& D, IQIndex& newmid, 
                IQTensor& C, IQTensor& U);

    Real 
    diag_denmat_complex(const IQTensor& rho, Vector& D, IQIndex& newmid, 
                        IQTensor& U);
    Real 
    diag_denmat_complex(const ITensor& rho, Vector& D, Index& newmid, 
                        ITensor& U);

    void 
    read(std::istream& s);
    void 
    write(std::ostream& s) const;


    void 
    svdRank2(const ITensor& A, ITensor& U, ITSparse& D, ITensor& V);

    void 
    svdRank2(const IQTensor& A, IQTensor& U, IQTSparse& D, IQTensor& V);

private:

    void
    diag_and_truncate(const IQTensor& rho, std::vector<Matrix>& mmatrix, 
                      std::vector<Vector>& mvector, std::vector<Real>& alleig, 
                      Real& svdtruncerr, IQIndex& newmid);
    void
    buildUnitary(const IQTensor& rho, const std::vector<Matrix>& mmatrix, 
                 const std::vector<Vector>& mvector,
                 const IQIndex& newmid, IQTensor& U);

    void
    buildCenter(const IQTensor& rho, const std::vector<Matrix>& mmatrix, 
                const std::vector<Vector>& mvector,
                const IQIndex& newmid, IQTensor& C);

    ITensor 
    pseudoInverse(const ITensor& C, Real cutoff = 0);

    IQTensor 
    pseudoInverse(const IQTensor& C, Real cutoff = 0);

    /////////////////
    //
    // Data Members
    //

    int N;
    std::vector<Real> truncerr_;
    Real cutoff_;
    int minm_;
    int maxm_;
    bool use_orig_m_; 
    bool showeigs_;
    bool doRelCutoff_;
    bool absoluteCutoff_;
    LogNumber refNorm_;
    std::vector<Vector> eigsKept_;
    Real noise_;

    //
    /////////////////

    }; //class SVDWorker

template<class Tensor, class SparseT, class LocalOpT>
void SVDWorker::
csvd(int b, const LocalOpT& PH, const Tensor& AA, 
     Tensor& L, SparseT& V, Tensor& R)
    {
    typedef typename Tensor::IndexT 
    IndexT;
    typedef typename Tensor::CombinerT 
    CombinerT;

    CombinerT Lcomb;
    Lcomb.doCondense(true);
    for(int j = 1; j <= L.r(); ++j) 
        { 
        const IndexT& I = L.index(j);

        if( V.hasindex(I) 
         || R.hasindex(I) 
         || I == Tensor::ReImIndex()) 
            continue;

        Lcomb.addleft(I);
        }

    CombinerT Rcomb;
    Rcomb.doCondense(true);
    for(int j = 1; j <= R.r(); ++j) 
        { 
        const IndexT& I = R.index(j);

        if( V.hasindex(I) 
         || L.hasindex(I) 
         || I == Tensor::ReImIndex()) 
            continue;

        Rcomb.addleft(I);
        }

    /*

    //Apply combiners
    comb.init(ll.rawname());

    Tensor AAc; 
    Lcomb.product(AA,AAc);
    Rcomb.product(AAc,AAc);

    const IndexT& active = comb.right();

    Tensor AAcc = conj(AAc); 
    AAcc.primeind(active); 
    Tensor rho = AAc*AAcc; 

    if(noise_ > 0 && PH.isNotNull())
        {
        //Direction dir = (ldim < rdim ? Fromleft : Fromright);
        //rho += noise_*PH.deltaRho(rho,comb,dir);
        rho += noise_*PH.deltaRho(rho,comb,Fromright);
        }

    const Real saved_cutoff = cutoff_; 
    const int saved_minm = minm_,
              saved_maxm = maxm_; 
    if(use_orig_m_)
        {
        cutoff_ = -1;
        minm_ = ll.m();
        maxm_ = ll.m();
        }

    Tensor C,U;
    if(AAc.isComplex())
        {
        Error("Complex case not implemented");
        }
    else
        truncerr_.at(b) = diag_denmat(rho,eigsKept_.at(b),ll,C,U);

    cutoff_ = saved_cutoff; 
    minm_ = saved_minm; 
    maxm_ = saved_maxm; 

    Oth = conj(U) * AAc;

    V = pseudoInverse(C);
    //V = pseudoInverse(C,0.01*sqrt(cutoff_));

    comb.conj();
    comb.product(U,Act);
    Act.conj(C.index(1));
    Act /= C;
    */

    } //void SVDWorker::csvd

template<class Tensor, class LocalOpT>
void SVDWorker::
denmatDecomp(int b, const LocalOpT& PH, const Tensor& AA, 
           Tensor& A, Tensor& B, Direction dir)
    {
    typedef typename Tensor::IndexT 
    IndexT;
    typedef typename Tensor::CombinerT 
    CombinerT;

    if(AA.vecSize() == 0) 
        {
        A *= 0;
        B *= 0;
        eigsKept_.at(b).ReDimension(1);
        eigsKept_.at(b) = 1;
        return;
        }

    IndexT mid = index_in_common(A,B,Link);
    if(mid.isNull()) mid = IndexT("mid");

    //If dir==None, put the O.C. on the side
    //that keeps mid's arrow the same
    bool do_edge_case = true;
    if(dir == None)
        {
        //std::cerr << boost::format("Arrow before = %s\n")%(mid.dir() == Out ? "Out" : "In");
        dir = (mid.dir() == Out ? Fromright : Fromleft);
        do_edge_case = false;
        }

    Tensor& to_orth = (dir==Fromleft ? A : B);
    Tensor& newoc   = (dir==Fromleft ? B : A);

    CombinerT comb;

    int unique_link = 0; //number of Links unique to to_orth
    for(int j = 1; j <= to_orth.r(); ++j) 
        { 
        const IndexT& I = to_orth.index(j);
        if(!(newoc.hasindex(I) || I == Tensor::ReImIndex() ))
            {
            if(I.type() == Link) ++unique_link;
            comb.addleft(I);
            }
        }

    //Check if we're at the edge
    if(unique_link == 0 && do_edge_case)
        {
        comb.init(mid.rawname());
        comb.product(AA,newoc);
        to_orth = comb; to_orth.conj();
        eigsKept_.at(b) = Vector(comb.right().m()); 
        eigsKept_.at(b) = 1.0/comb.right().m();
        return;
        }

    //Apply combiner
    comb.doCondense(true);
    comb.init(mid.rawname());
    Tensor AAc; comb.product(AA,AAc);

    const IndexT& active = comb.right();

    Tensor AAcc = conj(AAc); 
    AAcc.primeind(active); 
    Tensor rho = AAc*AAcc; 

    if(noise_ > 0 && PH.isNotNull())
        {
        rho += noise_*PH.deltaRho(rho,comb,dir);
        }

    const Real saved_cutoff = cutoff_; 
    const int saved_minm = minm_,
              saved_maxm = maxm_; 
    if(use_orig_m_)
        {
        cutoff_ = -1;
        minm_ = mid.m();
        maxm_ = mid.m();
        }

    IndexT newmid;
    Tensor U;
    if(AAc.isComplex())
        truncerr_.at(b) = diag_denmat_complex(rho,eigsKept_.at(b),newmid,U);
    else
        truncerr_.at(b) = diag_denmat(rho,eigsKept_.at(b),newmid,U);

    cutoff_ = saved_cutoff; 
    minm_ = saved_minm; 
    maxm_ = saved_maxm; 

    comb.conj();
    comb.product(U,to_orth);
    newoc = conj(U) * AAc;

    } //void SVDWorker::denmatDecomp

template<class Tensor, class SparseT, class LocalOpT>
void SVDWorker::
svd(int b, const LocalOpT& PH, const Tensor& AA, 
     Tensor& L, SparseT& V, Tensor& R)
    {
    typedef typename Tensor::IndexT 
    IndexT;
    typedef typename Tensor::CombinerT 
    CombinerT;

    } // void SVDWorker::svd


#endif
