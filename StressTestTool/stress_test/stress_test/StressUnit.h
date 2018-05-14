#pragma once

class CStressUnit
{
public:
    CStressUnit(void);
    ~CStressUnit(void);
private:
    void clean();
public:
    int SetBpCfg(const KcbpConfig& bp);
    int InitAll();
    int UninitAll();
};
