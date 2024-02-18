package woo.exceptions;

public class InsufficientStockException extends Exception {

	/**
	 * 
	 */
	private static final long serialVersionUID = 4353147383669868235L;
	
	private int _entrySpecification;
	
	public InsufficientStockException(int entrySpecification) {
	    _entrySpecification = entrySpecification;
	  }

	  /**
	   * @param entrySpecification
	   * @param cause
	   */
	  public InsufficientStockException(int entrySpecification, Exception cause) {
	    super(cause);
	    _entrySpecification = entrySpecification;
	  }

	  /**
	   * @return the bad entry specification.
	   */
	  public int getEntrySpecification() {
	    return _entrySpecification;
	  }

	
	
	
	
	
}
