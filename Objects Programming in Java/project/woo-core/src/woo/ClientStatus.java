package woo;

import java.io.Serializable;

public class ClientStatus implements Serializable {
	
	/**
	 * 
	 */
	private static final long serialVersionUID = 3655020532716226133L;
	private String _status;

	public ClientStatus() {
		_status = "NORMAL";
	}
	
	public void setElite () {
		_status = "ELITE";
	}
	
	public void setSelection () {
		_status = "SELECTION";
	}
	
	public void setNormal () {
		_status = "NORMAL";
	}
	
	public String currentStatus () {
		return _status;
	}
	

}
