/**
 */
package machineModel;

import org.eclipse.emf.ecore.EObject;

/**
 * <!-- begin-user-doc -->
 * A representation of the model object '<em><b>Event</b></em>'.
 * <!-- end-user-doc -->
 *
 * <p>
 * The following features are supported:
 * </p>
 * <ul>
 *   <li>{@link machineModel.Event#getName <em>Name</em>}</li>
 *   <li>{@link machineModel.Event#getFunctionToMonitor <em>Function To Monitor</em>}</li>
 * </ul>
 *
 * @see machineModel.MachineModelPackage#getEvent()
 * @model
 * @generated
 */
public interface Event extends EObject {
	/**
	 * Returns the value of the '<em><b>Name</b></em>' attribute.
	 * <!-- begin-user-doc -->
	 * <p>
	 * If the meaning of the '<em>Name</em>' attribute isn't clear,
	 * there really should be more of a description here...
	 * </p>
	 * <!-- end-user-doc -->
	 * @return the value of the '<em>Name</em>' attribute.
	 * @see #setName(String)
	 * @see machineModel.MachineModelPackage#getEvent_Name()
	 * @model
	 * @generated
	 */
	String getName();

	/**
	 * Sets the value of the '{@link machineModel.Event#getName <em>Name</em>}' attribute.
	 * <!-- begin-user-doc -->
	 * <!-- end-user-doc -->
	 * @param value the new value of the '<em>Name</em>' attribute.
	 * @see #getName()
	 * @generated
	 */
	void setName(String value);

	/**
	 * Returns the value of the '<em><b>Function To Monitor</b></em>' attribute.
	 * <!-- begin-user-doc -->
	 * <p>
	 * If the meaning of the '<em>Function To Monitor</em>' attribute isn't clear,
	 * there really should be more of a description here...
	 * </p>
	 * <!-- end-user-doc -->
	 * @return the value of the '<em>Function To Monitor</em>' attribute.
	 * @see #setFunctionToMonitor(String)
	 * @see machineModel.MachineModelPackage#getEvent_FunctionToMonitor()
	 * @model
	 * @generated
	 */
	String getFunctionToMonitor();

	/**
	 * Sets the value of the '{@link machineModel.Event#getFunctionToMonitor <em>Function To Monitor</em>}' attribute.
	 * <!-- begin-user-doc -->
	 * <!-- end-user-doc -->
	 * @param value the new value of the '<em>Function To Monitor</em>' attribute.
	 * @see #getFunctionToMonitor()
	 * @generated
	 */
	void setFunctionToMonitor(String value);

} // Event
