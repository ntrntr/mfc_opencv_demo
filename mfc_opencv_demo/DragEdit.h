#pragma once


// DragEdit

class DragEdit : public CEdit
{
	DECLARE_DYNAMIC(DragEdit)

public:
	DragEdit();
	virtual ~DragEdit();

protected:
	DECLARE_MESSAGE_MAP()
public:
	CString filePath;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDropFiles(HDROP hDropInfo);
};


