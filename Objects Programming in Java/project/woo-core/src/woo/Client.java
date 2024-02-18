package woo;

import java.io.Serializable;
import java.util.ArrayList;
import java.util.List;

public class Client implements Serializable{
	/**
	 * 
	 */
	private static final long serialVersionUID = 8206070447785147475L;
	private String _id;
	private String _address;
	private String _name;
	private ClientStatus _status;
	private int _salesvalue;
	private int _salespayed;
	private double _points = 0;
	private ArrayList<Integer> _transactions = new ArrayList<Integer>();
	
	
	public Client(String id, String name, String address) {
		_id = id;
		_name = name;
		_address = address;
		_status = new ClientStatus();
	}
	
	public String get_id() {
		return _id;
	}

	public String get_address() {
		return _address;
	}
	
	
	public String get_status() {
		return _status.currentStatus();
	}

	public int get_salesvalue() {
		return _salesvalue;
	}
	public void set_salesvalue(int salesvalue) {
		_salesvalue = salesvalue;
	}
	public int get_salespayed() {
		return _salespayed;
	}
	public void set_salespayed(int salespayed) {
		this._salespayed = salespayed;
	}
	public String get_name() {
		return _name;
	}
	public void set_name(String name) {
		this._name = name;
	}
	public double get_points() {
		return _points;
	}
	
	public void set_status_Selection() {
		_status.setSelection();
	}
	
	public void set_status_Normal() {
		_status.setNormal();
	}
	
	public void addTransaction(int transactionid) {
		_transactions.add(transactionid);
	}
	
	public void addPoints (double points) {
		_points += points;
		if(_points > 2000 ) {
			_status.setSelection();
		}
		if(_points > 2500) {
			_status.setElite();
		}
	}
	
	public void subtractPoints (double points) {
		_points -= points;
	}
	
	public void addSalesValue (double value) {
		_salesvalue += value;
	}
	
	public void addSalesPaid (double value) {
		_salespayed += value;
	}
	
	public ArrayList<Integer> printTransactions () {

		return _transactions;

	}
	
	
	@Override
	@SuppressWarnings("nls")
	public String toString() {
		// TODO Auto-generated method stub
		return _id + "|" + _name + "|" + _address + "|" + _status.currentStatus() + "|" + _salesvalue +"|" + _salespayed + "\n";
	}

}
