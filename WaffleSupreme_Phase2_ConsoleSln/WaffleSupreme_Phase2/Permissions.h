#pragma once

using namespace std;

class Permissions
{
public:
	Permissions()
	{

	}
	Permissions(bool r, bool w, bool access) :
		read(r),
		write(w),
		fileAccess(access)
	{

	}
private:
	bool read = false;
	bool write = false;
	bool fileAccess = false;
	//CommunicationLink cLink;
};