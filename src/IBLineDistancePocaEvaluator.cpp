#include "IBLineDistancePocaEvaluator.h"
#include <iostream>

class IBLineDistancePocaEvaluatorPimpl
{
public:
    IBLineDistancePocaEvaluatorPimpl()
    {
        m_integrity = true;
        m_cutlength = 0.f;
        m_poca << 0,0,0,1;
    }

    void evaluatePoCA()
    {
        HPoint3f p    = m_muon.LineIn().origin;
        HPoint3f q    = m_muon.LineOut().origin;
        Vector4f v    = getDirectorCosines(m_muon.LineIn().direction);
        Vector4f w    = getDirectorCosines(m_muon.LineOut().direction);
        Vector4f diff = q - p;
        Scalarf  prod = v.transpose() * w;
        Scalarf  den  = 1./(1. - prod*prod);
        if (unlikely(!isFinite(den))) {
            if (m_cutlength==0) {
                m_integrity = false;
                return;
            } else {
                float pr = diff.dot(v);
                Vector3f proj = (v * pr).head(3);
                Vector3f dist = diff.head(3);
                dist = proj.cross(dist);
                dist /= pr;
                if (dist.head(3).norm()>m_cutlength) {
                    m_integrity = false;
                    return;
                }
            }
        }
        else {
            Scalarf lambda = (diff.transpose()*(v*den-(w*prod*den)));
            Scalarf mu     = (diff.transpose()*((v*prod)*den-w*den));
            m_inPoca       = p + v*lambda;
            m_outPoca      = q + w*mu;
            Vector4f mdseg = (m_outPoca-m_inPoca);
            if (unlikely(m_cutlength != 0.f && mdseg.head(3).norm() > m_cutlength)) {
                m_integrity = false;
                return;
            }
            m_poca = m_inPoca + (mdseg*0.5);
            m_integrity   = true;
        }
    }

    HVector3f getDirectorCosines(const HVector3f &track_direction)
    {
        return track_direction / track_direction.head(3).norm();
    }

public:
    bool m_integrity;
    Scalarf m_cutlength;
    MuonScatterData m_muon;
    HPoint3f m_poca;
    HPoint3f m_inPoca;
    HPoint3f m_outPoca;
};


IBLineDistancePocaEvaluator::IBLineDistancePocaEvaluator() :
    d(new IBLineDistancePocaEvaluatorPimpl)
{
}

IBLineDistancePocaEvaluator::~IBLineDistancePocaEvaluator()
{
    delete d;
}

bool IBLineDistancePocaEvaluator::evaluate(MuonScatterData muon)
{
    d->m_muon = muon;
    d->m_integrity = true;
    d->evaluatePoCA();
    return d->m_integrity;
}

HPoint3f IBLineDistancePocaEvaluator::getPoca()
{
    return d->m_poca;
}

HPoint3f IBLineDistancePocaEvaluator::getInTrackPoca()
{
    return d->m_inPoca;
}

void IBLineDistancePocaEvaluator::setDistanceCut(Scalarf length)
{
    d->m_cutlength = length;
}

HPoint3f IBLineDistancePocaEvaluator::getOutTrackPoca()
{
    return d->m_outPoca;
}
