package woo.exceptions;

/**
 * Exception for unknown import file entries.
 */
public class UnknownSupplierException extends Exception {

  /** Class serial number. */

  /**
	 * 
	 */
	private static final long serialVersionUID = -4071018302781839365L;
/** Bad bad entry specification. */
  private String _entrySpecification;

  /**
   * @param entrySpecification
   */
  public UnknownSupplierException(String entrySpecification) {
    _entrySpecification = entrySpecification;
  }

  /**
   * @param entrySpecification
   * @param cause
   */
  public UnknownSupplierException(String entrySpecification, Exception cause) {
    super(cause);
    _entrySpecification = entrySpecification;
  }

  /**
   * @return the bad entry specification.
   */
  public String getEntrySpecification() {
    return _entrySpecification;
  }

}