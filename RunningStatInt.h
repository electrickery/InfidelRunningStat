#ifndef RunningStatInt_h
#define RunningStatInt_h

class RunningStatInt {

    public:
        void Clear();
        void Push(double x);
        int NumDataValues() const;
        double Mean() const;
        double Variance() const;
        double StandardDeviation() const;
        
    private:
        int m_n;
        double m_oldM, m_newM, m_oldS, m_newS;

};

#endif // RunningStatInt_h
