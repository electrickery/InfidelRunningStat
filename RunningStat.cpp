/* RunningStat - calculates mean, variance and standard deviation 
 *  without storing all values.
 *  
 * The implementation is copied from this page:
 *  https://www.johndcook.com/blog/standard_deviation/. 
 *  
 *  The Min() and Max() methods were added by me
 * 
 * fjkraan@electrickery.nl, 2021-08-25
 */
 
#include <Arduino.h>

class RunningStat
    {
    public:
        RunningStat() : m_n(0) {}

        void Clear()
        {
            m_n = 0;
        }

        void Push(double x)
        {
            m_n++;

            // See Knuth TAOCP vol 2, 3rd edition, page 232
            if (m_n == 1)
            {
                m_oldM = m_newM = x;
                m_oldS = m_newS = 0.0;
                
                // extra boundary values (added fjk)
                min = max = x;
            }
            else
            {
                m_newM = m_oldM + (x - m_oldM)/m_n;
                m_newS = m_oldS + (x - m_oldM)*(x - m_newM);
    
                // set up for next iteration
                m_oldM = m_newM; 
                m_oldS = m_newS;

                // extra boundary values (added fjk)
                min = (x < min) ? x : min;
                max = (x > max) ? x : max;

            }
        }

        int NumDataValues() const
        {
            return m_n;
        }

        double Mean() const
        {
            return (m_n > 0) ? m_newM : 0.0;
        }

        double Variance() const
        {
            return ( (m_n > 1) ? m_newS/(m_n - 1) : 0.0 );
        }

        double StandardDeviation() const
        {
            return sqrt( Variance() );
        }

        double Min() const
        {
            return min;
        }
        
        double Max() const
        {
            return max;
        }

    private:
        int m_n;
        double m_oldM, m_newM, m_oldS, m_newS;
        double min, max;
    };
