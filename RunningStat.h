#ifndef RunningStat_h
#define RunningStat_h

class RunningStatInt {

    public:
        void Clear();
        void Push(double x);
        int NumDataValues() const;
        double Mean() const;
        double Variance() const;
        double StandardDeviation() const;
        double Min();
        double Max();
        
    private:
        int m_n;
        double m_oldM, m_newM, m_oldS, m_newS;
        double min, max;
};

#endif // RunningStat_h
