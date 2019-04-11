#pragma once

enum {
	ID_DILLER,
	ID_PLAYER1,
	ID_PLAYER2,
	ID_PLAYER3,
	ID_FOR_EVERY
};

template < typename T >
class Singleton {
private:
public:
	int mType;
	int GetType() {return mType; };
	static T& GetInstance(int type) {
		T * ret_instance = nullptr;
		switch (type)
		{
		case ID_DILLER:
			if (!m_instance_Diller) {
				m_instance_Diller = new T();
			}
			ret_instance = m_instance_Diller;
			ret_instance->mType = type;
			break;
		case ID_PLAYER1:
			if (!m_instance_Player1) {
				m_instance_Player1 = new T();
			}
			ret_instance = m_instance_Player1;
			ret_instance->mType = type;
			break;

		case ID_PLAYER2:
			if (!m_instance_Player2) {
				m_instance_Player2 = new T();
			}
			ret_instance = m_instance_Player2;
			ret_instance->mType = type;
			break;
		case ID_PLAYER3:
			if (!m_instance_Player3) {
				m_instance_Player3 = new T();
			}
			ret_instance = m_instance_Player3;
			m_instance_Diller->mType = type;
			break;

		default:
			ret_instance = nullptr;
			break;
		}
		return *ret_instance;
	}

	Singleton(const Singleton&) = delete;
	Singleton& operator= (const Singleton) = delete;

protected:
	Singleton() { mType= ID_DILLER; };
	Singleton(int Type) { mType = Type;};
	virtual ~Singleton() {
		switch (mType)
		{
		case ID_DILLER:
			delete m_instance_Diller;
			m_instance_Diller = nullptr;
			break;
		case ID_PLAYER1:
			delete m_instance_Player1;
			m_instance_Player1 = nullptr;
			break;
		case ID_PLAYER2:
			delete m_instance_Player2;
			m_instance_Player2 = nullptr;
			break;
		case ID_PLAYER3:
			delete m_instance_Player3;
			m_instance_Player3 = nullptr;
			break;
		default:
			//ASSERT(0)
			break;
		}
	}

private:
	inline static T * m_instance_Diller = nullptr;
	inline static T * m_instance_Player1 = nullptr;
	inline static T * m_instance_Player2 = nullptr;
	inline static T * m_instance_Player3 = nullptr;
};