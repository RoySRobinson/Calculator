#pragma once
#include <vector>

class Calculator
{
public:
	// ActionType: what has been entered from the calculator
	enum class ActionType : char {Number, Plus, Minus, Multiply, Divide, Equals, None};
	// 
	struct Action
	{
		ActionType actionType;
		double value;
	};

	void reset();
	bool addInput(const Action& input);
	Action getLastInput() const;
	// Current (partial) result as much as it can be calculated. Terms results are not
	// taken into account until the term has finished: 3 + 2 X 5 would return 3 becouse
	// the term calculation is not finished.
	double getCurrentResult() const;
	bool hasLeftTermValue() const { return m_leftTerm.hasValue(); }
	bool hasLeftExpressionValue() const { return m_leftExpression.hasValue(); }
	bool isOperation(ActionType action) const;
	// actions
	int getActionsSize() { return static_cast<int>(m_actions.size()); }
	const Action& getAction(int i) { return m_actions.at(i);  }
private:
	bool isTerm(ActionType action) const;
	bool isExpression(ActionType action) const;
	// like: +,-
	class LeftExpression
	{
	public:
		void reset();
		void set(double value, ActionType rightOperation);
		void add(double value, ActionType rightOperation);
		double getValue() const { return m_value; }
		bool hasValue() const { return m_hasValue; }
	private:
		bool m_hasValue = false;
		double m_value = 0.0;
		ActionType m_rightOperation = ActionType::None;
	};
	// like: x,/
	class LeftTerm
	{
	public:
		void reset();
		void set(double value, ActionType rightOperation);
		void multiplyBy(double value, ActionType rightOperation);
		double getValue() const { return m_value; }
		bool hasValue() const { return m_hasValue; }
	private:
		bool m_hasValue = false;
		double m_value;
		ActionType m_rightOperation = ActionType::None;
	};
	ActionType getLastOperation();
	std::vector<Action> m_actions; // all the actions user has inputted (see ActionType)
	// m_leftExpression is always the left hand side of the expression. An example:
	// 5 -> m_leftExpression = 5 (*)
	// 5 + 3 + -> m_leftExpression = 8
	// 5 + 3 + 1 = -> m_leftExpression = 9
	// 5 + 3 + 1 = 9 + -> m_leftExpression = 9 (here "=" the user must add 9
	// before using "+" becouse "=" "resets" the calculations and they must
	// start from the beginning, meaning a number is entered first).
	// So "=" and "None" (see (*)) are the start or the calculations.
	LeftExpression m_leftExpression;
	// if the calculation starts with terms:
	// 3 x 4, this will go to m_leftTerm (not m_leftExpression). So m_leftExpression
	// stays zero until next expression comes.
	LeftTerm m_leftTerm;
};

inline void Calculator::LeftExpression::reset() 
{
	m_hasValue = false;
	m_value = 0.0;
	m_rightOperation = ActionType::None;
}

inline void Calculator::LeftExpression::set(double value, ActionType rightOperation) 
{
	m_value = value;
	m_rightOperation = rightOperation;
	m_hasValue = true;
}

inline void Calculator::LeftExpression::add(double value, ActionType rightOperation)
{
	set(m_value + value, rightOperation);
}

inline void Calculator::LeftTerm::reset() 
{
	m_rightOperation = ActionType::None;
	m_value = 0.0;
	m_hasValue = false;
}

inline void Calculator::LeftTerm::set(double value, ActionType rightOperation) 
{
	m_value = value;
	m_rightOperation = rightOperation;
	m_hasValue = true;
}

inline void Calculator::LeftTerm::multiplyBy(double value, ActionType rightOperation)
{
	set(m_value * value, rightOperation);
}

