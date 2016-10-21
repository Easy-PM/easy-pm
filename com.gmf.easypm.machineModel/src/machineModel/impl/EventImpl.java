/**
 */
package machineModel.impl;

import machineModel.Event;
import machineModel.MachineModelPackage;

import org.eclipse.emf.common.notify.Notification;

import org.eclipse.emf.ecore.EClass;

import org.eclipse.emf.ecore.impl.ENotificationImpl;
import org.eclipse.emf.ecore.impl.MinimalEObjectImpl;

/**
 * <!-- begin-user-doc -->
 * An implementation of the model object '<em><b>Event</b></em>'.
 * <!-- end-user-doc -->
 * <p>
 * The following features are implemented:
 * </p>
 * <ul>
 *   <li>{@link machineModel.impl.EventImpl#getName <em>Name</em>}</li>
 *   <li>{@link machineModel.impl.EventImpl#getFunctionToMonitor <em>Function To Monitor</em>}</li>
 * </ul>
 *
 * @generated
 */
public class EventImpl extends MinimalEObjectImpl.Container implements Event {
	/**
	 * The default value of the '{@link #getName() <em>Name</em>}' attribute.
	 * <!-- begin-user-doc -->
	 * <!-- end-user-doc -->
	 * @see #getName()
	 * @generated
	 * @ordered
	 */
	protected static final String NAME_EDEFAULT = null;

	/**
	 * The cached value of the '{@link #getName() <em>Name</em>}' attribute.
	 * <!-- begin-user-doc -->
	 * <!-- end-user-doc -->
	 * @see #getName()
	 * @generated
	 * @ordered
	 */
	protected String name = NAME_EDEFAULT;

	/**
	 * The default value of the '{@link #getFunctionToMonitor() <em>Function To Monitor</em>}' attribute.
	 * <!-- begin-user-doc -->
	 * <!-- end-user-doc -->
	 * @see #getFunctionToMonitor()
	 * @generated
	 * @ordered
	 */
	protected static final String FUNCTION_TO_MONITOR_EDEFAULT = null;

	/**
	 * The cached value of the '{@link #getFunctionToMonitor() <em>Function To Monitor</em>}' attribute.
	 * <!-- begin-user-doc -->
	 * <!-- end-user-doc -->
	 * @see #getFunctionToMonitor()
	 * @generated
	 * @ordered
	 */
	protected String functionToMonitor = FUNCTION_TO_MONITOR_EDEFAULT;

	/**
	 * <!-- begin-user-doc -->
	 * <!-- end-user-doc -->
	 * @generated
	 */
	protected EventImpl() {
		super();
	}

	/**
	 * <!-- begin-user-doc -->
	 * <!-- end-user-doc -->
	 * @generated
	 */
	@Override
	protected EClass eStaticClass() {
		return MachineModelPackage.Literals.EVENT;
	}

	/**
	 * <!-- begin-user-doc -->
	 * <!-- end-user-doc -->
	 * @generated
	 */
	public String getName() {
		return name;
	}

	/**
	 * <!-- begin-user-doc -->
	 * <!-- end-user-doc -->
	 * @generated
	 */
	public void setName(String newName) {
		String oldName = name;
		name = newName;
		if (eNotificationRequired())
			eNotify(new ENotificationImpl(this, Notification.SET, MachineModelPackage.EVENT__NAME, oldName, name));
	}

	/**
	 * <!-- begin-user-doc -->
	 * <!-- end-user-doc -->
	 * @generated
	 */
	public String getFunctionToMonitor() {
		return functionToMonitor;
	}

	/**
	 * <!-- begin-user-doc -->
	 * <!-- end-user-doc -->
	 * @generated
	 */
	public void setFunctionToMonitor(String newFunctionToMonitor) {
		String oldFunctionToMonitor = functionToMonitor;
		functionToMonitor = newFunctionToMonitor;
		if (eNotificationRequired())
			eNotify(new ENotificationImpl(this, Notification.SET, MachineModelPackage.EVENT__FUNCTION_TO_MONITOR, oldFunctionToMonitor, functionToMonitor));
	}

	/**
	 * <!-- begin-user-doc -->
	 * <!-- end-user-doc -->
	 * @generated
	 */
	@Override
	public Object eGet(int featureID, boolean resolve, boolean coreType) {
		switch (featureID) {
			case MachineModelPackage.EVENT__NAME:
				return getName();
			case MachineModelPackage.EVENT__FUNCTION_TO_MONITOR:
				return getFunctionToMonitor();
		}
		return super.eGet(featureID, resolve, coreType);
	}

	/**
	 * <!-- begin-user-doc -->
	 * <!-- end-user-doc -->
	 * @generated
	 */
	@Override
	public void eSet(int featureID, Object newValue) {
		switch (featureID) {
			case MachineModelPackage.EVENT__NAME:
				setName((String)newValue);
				return;
			case MachineModelPackage.EVENT__FUNCTION_TO_MONITOR:
				setFunctionToMonitor((String)newValue);
				return;
		}
		super.eSet(featureID, newValue);
	}

	/**
	 * <!-- begin-user-doc -->
	 * <!-- end-user-doc -->
	 * @generated
	 */
	@Override
	public void eUnset(int featureID) {
		switch (featureID) {
			case MachineModelPackage.EVENT__NAME:
				setName(NAME_EDEFAULT);
				return;
			case MachineModelPackage.EVENT__FUNCTION_TO_MONITOR:
				setFunctionToMonitor(FUNCTION_TO_MONITOR_EDEFAULT);
				return;
		}
		super.eUnset(featureID);
	}

	/**
	 * <!-- begin-user-doc -->
	 * <!-- end-user-doc -->
	 * @generated
	 */
	@Override
	public boolean eIsSet(int featureID) {
		switch (featureID) {
			case MachineModelPackage.EVENT__NAME:
				return NAME_EDEFAULT == null ? name != null : !NAME_EDEFAULT.equals(name);
			case MachineModelPackage.EVENT__FUNCTION_TO_MONITOR:
				return FUNCTION_TO_MONITOR_EDEFAULT == null ? functionToMonitor != null : !FUNCTION_TO_MONITOR_EDEFAULT.equals(functionToMonitor);
		}
		return super.eIsSet(featureID);
	}

	/**
	 * <!-- begin-user-doc -->
	 * <!-- end-user-doc -->
	 * @generated
	 */
	@Override
	public String toString() {
		if (eIsProxy()) return super.toString();

		StringBuffer result = new StringBuffer(super.toString());
		result.append(" (name: ");
		result.append(name);
		result.append(", functionToMonitor: ");
		result.append(functionToMonitor);
		result.append(')');
		return result.toString();
	}

} //EventImpl
