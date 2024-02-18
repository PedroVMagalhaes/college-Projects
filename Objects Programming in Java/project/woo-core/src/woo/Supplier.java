package woo;

import java.io.Serializable;

public class Supplier implements Serializable {		
	/**
	 * 
	 */
	private static final long serialVersionUID = -5519841420523565702L;
	private String _id;
	private String _address;
	private String _name;
	private String _activity;
	
	
	public Supplier(String id, String name, String address) {
		_id = id;
		_address = address;
		_name = name;
		_activity = "SIM";
	}


	public String get_id() {
		return _id;
	}



	public String get_address() {
		return _address;
	}


	public String get_name() {
		return _name;
	}


	public String get_activity() {
		return _activity;
	}
	
	public Boolean change_activity() {
		if (_activity.equals("SIM")) {
			_activity = "NÃO";
			return false;
		}
		else {
			_activity = "SIM";
			return true;
		}
	}


	@Override
	@SuppressWarnings("nls")
	public String toString() {
		// TODO Auto-generated method stub
		return _id + "|" + _name + "|" + _address + "|" + _activity + "\n";
	}
}
