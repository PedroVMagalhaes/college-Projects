package woo.exceptions;

/**
 * Exception for unknown import file entries.
 */
public class WrongServiceLevelException extends Exception {

  /** Class serial number. */
  private static final long serialVersionUID = 201409301049L;

  /** Bad bad entry specification. */
  private String _entrySpecification;

  /**
   * @param entrySpecification
   */
  public WrongServiceLevelException(String entrySpecification) {
    _entrySpecification = entrySpecification;
  }

  /**
   * @param entrySpecification
   * @param cause
   */
  public WrongServiceLevelException(String entrySpecification, Exception cause) {
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