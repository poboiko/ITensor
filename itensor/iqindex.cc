//
// Distributed under the ITensor Library License, Version 1.0.
//    (See accompanying LICENSE file.)
//
#include "iqindex.h"

using namespace std;
using boost::format;
using boost::array;
using boost::shared_ptr;
using boost::make_shared;

//
// IQIndexDat
//

class IQIndexDat
    {
    public:

    typedef vector<inqn>
    StorageT;

    typedef StorageT::iterator
    iterator;
    typedef StorageT::const_iterator
    const_iterator;


    IQIndexDat() { }

    IQIndexDat(const Index& i1, const QN& q1,
               const Index& i2 = Index::Null(), const QN& q2 = QN(),
               const Index& i3 = Index::Null(), const QN& q3 = QN(),
               const Index& i4 = Index::Null(), const QN& q4 = QN(),
               const Index& i5 = Index::Null(), const QN& q5 = QN(),
               const Index& i6 = Index::Null(), const QN& q6 = QN(),
               const Index& i7 = Index::Null(), const QN& q7 = QN(),
               const Index& i8 = Index::Null(), const QN& q8 = QN());

    explicit
    IQIndexDat(vector<inqn>& ind_qn);

    IQIndexDat(istream& s);

    const StorageT&
    iq() const { return iq_; }

    int
    size() { return iq_.size(); }

    const Index&
    index(int i) { return iq_[i-1].index; }
    const QN&
    qn(int i) { return iq_[i-1].qn; }

    iterator
    begin() { return iq_.begin(); }
    iterator
    end() { return iq_.end(); }

    const_iterator
    begin() const { return iq_.begin(); }
    const_iterator
    end()   const { return iq_.end(); }

    void 
    write(ostream& s) const;

    void 
    read(istream& s);

    static const IQIndexDatPtr& Null();

    static const IQIndexDatPtr& ReImDat();

    static const IQIndexDatPtr& ReImDatP();

    static const IQIndexDatPtr& ReImDatPP();

    void
    makeCopyOf(const IQIndexDat& other);

    private:

    //////////////////

    StorageT iq_;

    /////////////////

    //Disallow copying using =
    void 
    operator=(const IQIndexDat&);

    };

IQIndexDat::
IQIndexDat(const Index& i1, const QN& q1,
           const Index& i2, const QN& q2,
           const Index& i3, const QN& q3,
           const Index& i4, const QN& q4,
           const Index& i5, const QN& q5,
           const Index& i6, const QN& q6,
           const Index& i7, const QN& q7,
           const Index& i8, const QN& q8)
    {
    iq_.push_back(inqn(i1,q1));
    if(i2 != Index::Null())
        iq_.push_back(inqn(i2,q2));
    if(i3 != Index::Null())
        iq_.push_back(inqn(i3,q3));
    if(i4 != Index::Null())
        iq_.push_back(inqn(i4,q4));
    if(i5 != Index::Null())
        iq_.push_back(inqn(i5,q5));
    if(i6 != Index::Null())
        iq_.push_back(inqn(i6,q6));
    if(i7 != Index::Null())
        iq_.push_back(inqn(i7,q7));
    if(i8 != Index::Null())
        iq_.push_back(inqn(i8,q8));
    }

IQIndexDat::
IQIndexDat(StorageT& ind_qn)
    { 
    iq_.swap(ind_qn); 
    }

void IQIndexDat::
makeCopyOf(const IQIndexDat& other) 
    { 
    iq_ = other.iq_;
    }

IQIndexDat::
IQIndexDat(istream& s) 
    { read(s); }

void IQIndexDat::
write(ostream& s) const
    {
    size_t size = iq_.size();
    s.write((char*)&size,sizeof(size));
    Foreach(const inqn& x, iq_)
        { 
        x.write(s); 
        }
    }

void IQIndexDat::
read(istream& s)
    {
    size_t size; s.read((char*)&size,sizeof(size));
    iq_.resize(size);
    Foreach(inqn& x, iq_)
        { 
        x.read(s); 
        }
    }

const IQIndexDatPtr& IQIndexDat::
Null()
    {
    static IQIndexDatPtr Null_ = make_shared<IQIndexDat>(Index::Null(),QN());
    return Null_;
    }

const IQIndexDatPtr& IQIndexDat::
ReImDat()
    {
    static IQIndexDatPtr ReImDat_ = make_shared<IQIndexDat>(Index::IndReIm(),QN());
    return ReImDat_;
    }

const IQIndexDatPtr& IQIndexDat::
ReImDatP()
    {
    static IQIndexDatPtr ReImDatP_ = make_shared<IQIndexDat>(Index::IndReImP(),QN());
    return ReImDatP_;
    }

const IQIndexDatPtr& IQIndexDat::
ReImDatPP()
    {
    static IQIndexDatPtr ReImDatPP_ = make_shared<IQIndexDat>(Index::IndReImPP(),QN());
    return ReImDatPP_;
    }

//
// IQIndex Methods
//

#ifdef DEBUG
#define IQINDEX_CHECK_NULL if(pd == 0) Error("IQIndex is null");
#else
#define IQINDEX_CHECK_NULL
#endif

const IQIndexDat::StorageT& IQIndex::
iq() const 
    { 
    IQINDEX_CHECK_NULL
    return pd->iq();
    }

int IQIndex::
nindex() const 
    { 
    IQINDEX_CHECK_NULL
    return (int) pd->size(); 
    }

const Index& IQIndex::
index(int i) const 
    {
    IQINDEX_CHECK_NULL
#ifdef DEBUG
    if(i > nindex())
        {
        Print(nindex());
        Print(i);
        Error("IQIndex::index arg out of range");
        }
#endif
    return pd->index(i);
    }

const QN& IQIndex::
qn(int i) const 
    {
    IQINDEX_CHECK_NULL
#ifdef DEBUG
    if(i > nindex())
        {
        Print(nindex());
        Print(i);
        Error("IQIndex::qn arg out of range");
        }
#endif
    return pd->qn(i);
    }

IQIndex::
IQIndex() 
    : 
    _dir(Neither)
    { }


IQIndex::
IQIndex(const string& name,
                 IndexType it, 
                 Arrow dir, 
                 int plev) 
    : 
    index_(name,1,it,plev), 
    _dir(dir)
    { }

IQIndex::
IQIndex(const string& name, 
        const Index& i1, const QN& q1, 
        Arrow dir) 
    : 
    index_(name,i1.m(),i1.type(),i1.primeLevel()), 
    _dir(dir), 
    pd(make_shared<IQIndexDat>(i1,q1))
    {
    }

IQIndex::
IQIndex(const string& name, 
        const Index& i1, const QN& q1, 
        const Index& i2, const QN& q2,
        Arrow dir) 
    : 
    index_(name,i1.m()+i2.m(),i1.type(),i1.primeLevel()), 
    _dir(dir), 
    pd(make_shared<IQIndexDat>(i1,q1,i2,q2))
    {
    if(i2.type() != i1.type())
        Error("Indices must have the same type");
    }

IQIndex::
IQIndex(const string& name, 
        const Index& i1, const QN& q1, 
        const Index& i2, const QN& q2,
        const Index& i3, const QN& q3,
        Arrow dir) 
    : 
    index_(name,i1.m()+i2.m()+i3.m(),i1.type(),i1.primeLevel()), 
    _dir(dir),
    pd(make_shared<IQIndexDat>(i1,q1,i2,q2,i3,q3))
    {
    if(i2.type() != i1.type() 
    || i3.type() != i1.type())
        Error("Indices must have the same type");
    }

IQIndex::
IQIndex(const string& name, 
        const Index& i1, const QN& q1, 
        const Index& i2, const QN& q2,
        const Index& i3, const QN& q3,
        const Index& i4, const QN& q4,
        Arrow dir) 
    : 
    index_(name,i1.m()+i2.m()+i3.m()+i4.m(),i1.type(),i1.primeLevel()), 
    _dir(dir),
    pd(make_shared<IQIndexDat>(i1,q1,i2,q2,i3,q3,i4,q4))
    {
    if(i2.type() != i1.type() 
    || i3.type() != i1.type()
    || i4.type() != i1.type())
        Error("Indices must have the same type");
    }

IQIndex::
IQIndex(const string& name, 
        const Index& i1, const QN& q1, 
        const Index& i2, const QN& q2,
        const Index& i3, const QN& q3,
        const Index& i4, const QN& q4,
        const Index& i5, const QN& q5,
        Arrow dir) 
    : 
    index_(name,i1.m()+i2.m()+i3.m()+i4.m()+i5.m(),i1.type(),i1.primeLevel()), 
    _dir(dir),
    pd(new IQIndexDat(i1,q1,i2,q2,i3,q3,i4,q4,i5,q5))
    {
    if(i2.type() != i1.type() 
    || i3.type() != i1.type()
    || i4.type() != i1.type()
    || i5.type() != i1.type())
        Error("Indices must have the same type");
    }

IQIndex::
IQIndex(const string& name, 
        IQIndexDat::StorageT& ind_qn, 
        Arrow dir, int plev) 
    : 
    _dir(dir), 
    pd(new IQIndexDat(ind_qn))
    { 
    int mm = 0;
    Foreach(const inqn& iq, *pd)
        {
        mm += iq.index.m();
        if(iq.index.type() != index(1).type())
            Error("Indices must have the same type");
        }
    index_ = Index(name,mm,index(1).type(),plev);
    }

IQIndex::
IQIndex(const IQIndex& other, 
        IQIndexDat::StorageT& ind_qn)
    : 
    _dir(other._dir), 
    pd(new IQIndexDat(ind_qn))
    { 
    int mm = 0;
    Foreach(const inqn& iq, *pd)
        {
        mm += iq.index.m();
        if(iq.index.type() != index(1).type())
            Error("Indices must have the same type");
        }
    index_ = Index(other.name(),mm,other.type(),
                   pd->begin()->index.primeLevel()); 
    }

IQIndex::
IQIndex(const Index& other, 
        const Index& i1, const QN& q1, 
        Arrow dir) 
    : 
    index_(other),
    _dir(dir), 
    pd(make_shared<IQIndexDat>(i1,q1))
    {
    index_.primeLevel(i1.primeLevel());
    }

IQIndex::
IQIndex(const Index& index, const IQIndexDatPtr& pdat)
    : 
    index_(index),
    _dir(In),
    pd(pdat)
    { }

void IQIndex::
write(ostream& s) const
    {
    IQINDEX_CHECK_NULL
    index_.write(s);
    s.write((char*)&_dir,sizeof(_dir));
    pd->write(s);
    }

void IQIndex::
read(istream& s)
    {
    index_.read(s);
    s.read((char*)&_dir,sizeof(_dir));
    pd = make_shared<IQIndexDat>();
    pd->read(s);
    }

int IQIndex::
biggestm() const
    {
    IQINDEX_CHECK_NULL
    int mm = 0;
    Foreach(const inqn& iq, *pd)
        { 
        mm = max(mm,iq.index.m()); 
        }
    return mm;
    }

string
showm(const IQIndex& I)
    {
#ifdef DEBUG
    if(I.isNull()) Error("Null IQIndex");
#endif
    string res = " ";
    ostringstream oh; 
    oh << I.m() << " | ";
    Foreach(const inqn& iq, I.iq())
        {
        oh << boost::format("[%d,%d,%s]:%d ") % iq.qn.sz() % iq.qn.Nf() % (iq.qn.sign()==1?"+":"-") % iq.index.m(); 
        }
    return oh.str();
    }

QN IQIndex::
qn(const Index& i) const
    { 
    IQINDEX_CHECK_NULL
    Foreach(const inqn& iq, *pd)
        { 
        if(iq.index == i) 
            return iq.qn; 
        }
    cerr << *this << "\n";
    cerr << "i = " << i << "\n";
    Error("IQIndex::qn(Index): IQIndex does not contain given index.");
    return QN();
    }

Arrow IQIndex::
dir() const { return _dir; }

void IQIndex::
conj() { _dir = -_dir; }

const Index& IQIndex::
findbyqn(QN q) const
    { 
    IQINDEX_CHECK_NULL
    Foreach(const inqn& iq, *pd)
        {
        if(iq.qn == q) 
            return iq.index;
        }
    Error("IQIndex::findbyqn: no Index had a matching QN.");
    return Index::Null();
    }

bool IQIndex::
hasindex(const Index& i) const
    { 
    IQINDEX_CHECK_NULL
    Foreach(const inqn& iq, *pd)
        {
        if(iq.index == i) 
            return true;
        }
    return false;
    }

bool IQIndex::
hasindex_noprime(const Index& i) const
    { 
    IQINDEX_CHECK_NULL
    Foreach(const inqn& iq, *pd)
        {
        if(iq.index.noprimeEquals(i)) 
            return true;
        }
    return false;
    }

int IQIndex::
offset(const Index& I) const
    {
    int os = 0;
    Foreach(const inqn& iq, *pd)
        {
        if(iq.index == I) return os;
        os += iq.index.m();
        }
    Print(*this);
    Print(I);
    Error("Index not contained in IQIndex");
    return 0;
    }

void IQIndex::
primeLevel(int val)
    {
    solo();
    index_.primeLevel(val);
    Foreach(inqn& iq, *pd)
        iq.index.primeLevel(val);
    }

void IQIndex::
prime(int inc)
    {
    solo();
    index_.prime(inc);
    Foreach(inqn& iq, *pd)
        iq.index.prime(inc);
    }

void IQIndex::
prime(IndexType type, int inc)
    {
    solo();
    index_.prime(type,inc);
    Foreach(inqn& iq, *pd)
        iq.index.prime(type,inc);
    }

void IQIndex::
mapprime(int plevold, int plevnew, IndexType type)
    {
    solo();
    index_.mapprime(plevold,plevnew,type);
    Foreach(inqn& iq, *pd)
        iq.index.mapprime(plevold,plevnew,type);
    }

void IQIndex::
noprime(IndexType type)
    {
    solo();
    index_.noprime(type);
    Foreach(inqn& iq, *pd)
        iq.index.noprime(type);
    }


void IQIndex::
print(string name) const
    { cerr << "\n" << name << " =\n" << *this << "\n"; }

void IQIndex::
solo()
    {
    IQINDEX_CHECK_NULL
    if(!pd.unique())
        {
        const IQIndexDat& olddat = *pd;
        pd = make_shared<IQIndexDat>();
        pd->makeCopyOf(olddat);
        }
    }

const IQIndex& IQIndex::
Null()
    {
    static const IQIndex Null_(Index::Null(),IQIndexDat::Null());
    return Null_;
    }

const IQIndex& IQIndex::
IndReIm()
    {
    static const IQIndex IndReIm_(Index::IndReIm(),IQIndexDat::ReImDat());
    return IndReIm_;
    }

const IQIndex& IQIndex::
IndReImP()
    {
    static const IQIndex IndReImP_(Index::IndReImP(),IQIndexDat::ReImDatP());
    return IndReImP_;
    }

const IQIndex& IQIndex::
IndReImPP()
    {
    static const IQIndex IndReImPP_(Index::IndReImPP(),IQIndexDat::ReImDatPP());
    return IndReImPP_;
    }


ostream& 
operator<<(ostream &o, const IQIndex& I)
    {
    if(I.isNull()) 
        { 
        o << "IQIndex: (null)"; 
        return o;
        }
    o << "IQIndex: " << I.index_ << " <" << I.dir() << ">" << endl;
    for(int j = 1; j <= I.nindex(); ++j) 
        o << " " << I.index(j) SP I.qn(j) << "\n";
    return o;
    }


IQIndexVal::
IQIndexVal()
    : iqind(IQIndex::Null()), i(1) 
    { }


IQIndexVal::
IQIndexVal(const IQIndex& iqindex, int i_) 
    : 
    iqind(iqindex),
    i(i_) 
    { 
    if(i > iqind.m() || i < 1) 
        {
        Print(iqindex);
        Print(i);
        Error("IQIndexVal: i out of range");
        }
    }


Index IQIndexVal::
index() const 
    { 
    int j,ii;
    calc_ind_ii(j,ii);
    return iqind.index(j);
    }


QN IQIndexVal::
qn() const 
    { 
    int j,ii;
    calc_ind_ii(j,ii);
    return iqind.qn(j);
    }

bool IQIndexVal::
operator==(const IQIndexVal& other) const
    {
    return (iqind == other.iqind && i == other.i);
    }

IQIndexVal::
operator IndexVal() const 
    { 
    return IndexVal(Index(iqind),i); 
    }


IndexVal IQIndexVal::
blockIndexVal() const 
    { 
    if(*this == IQIndexVal::Null())
        return IndexVal::Null();
    int j,ii;
    calc_ind_ii(j,ii);
    return IndexVal(iqind.index(j),ii); 
    }

/*

IQIndexVal::
operator ITensor() const 
    { 
    return ITensor(IndexVal(iqind,i)); 
    }
*/


void IQIndexVal::
calc_ind_ii(int& j, int& ii) const
    {
    j = 1;
    ii = i;
    while(ii > iqind.index(j).m())
        {
        ii -= iqind.index(j).m();
        ++j;
        }
    }

IQIndexVal IQIndex::
operator()(int n) const 
    { return IQIndexVal(*this,n); }
