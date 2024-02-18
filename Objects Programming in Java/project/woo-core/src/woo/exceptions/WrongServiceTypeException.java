package woo.exceptions;

/**
 * Exception for unknown import file entries.
 */
public class WrongServiceTypeException extends Exception {

  /** Class serial number. */
  private static final long serialVersionUID = 201409301050L;

  /** Bad bad entry specification. */
  private String _entrySpecification;

  /**
   * @param entrySpecification
   */
  public WrongServiceTypeException(String entrySpecification) {
    _entrySpecification = entrySpecification;
  }

  /**
   * @param entrySpecification
   * @param cause
   */
  public WrongServiceTypeException(String entrySpecification, Exception cause) {
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