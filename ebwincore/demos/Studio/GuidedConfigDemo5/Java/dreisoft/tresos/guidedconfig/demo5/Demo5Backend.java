/*
 * Copyright (C) Elektrobit Automotive GmbH
 * All rights reserved
 */

package dreisoft.tresos.guidedconfig.demo5;

import dreisoft.tresos.guidedconfig.api.backend.AbstractBackend;
import dreisoft.tresos.guidedconfig.api.backend.ValidationResult;
import dreisoft.tresos.guidedconfig.api.memento.Memento;
import dreisoft.tresos.guidedconfig.demo5.generated.operationstatus.MyGuidedConfigOperationStatus;
import dreisoft.tresos.lib2.api.log.APIOperationStatus;

import java.util.ArrayList;
import java.util.List;

/**
 * This class is the controller of a wizard. It validates the widgets.
 *
 * <p>
 * <i> Copyright (C) Elektrobit Automotive GmbH - All rights reserved</i>
 * </p>
 *
 * @since 2009.a
 */
public class Demo5Backend extends AbstractBackend
{

    /**
     * This method is called asynchronously every time the model of the wizard is changed to query if the model has
     * errors. <br>
     * The following values are checked:<br>
     * <ul>
     * <li>an error is reported, if the Integer is empty</li>
     * <li>an error is reported, if the Float is empty</li>
     * <li>a warning is reported for each table row, if the Boolean value is false</li>
     * </ul>
     */
    @Override
    public List<ValidationResult> doValidate()
    {
        ArrayList<ValidationResult> result = new ArrayList<ValidationResult>();
        Memento rootMemento = getMemento();

        // add an error if the Integer field is empty
        String value = rootMemento.getString(IDemo5Constants.MEMENTO_KEY_TEXT);
        if ((value == null) || "".equals(value))
        {
            result.add(new ValidationResult(
                IDemo5Constants.MEMENTO_KEY_TEXT,
                MyGuidedConfigOperationStatus.ERROR_EMPTY_INTEGER()));
        }

        // add an error if the Float field is empty
        value = rootMemento.getString(IDemo5Constants.MEMENTO_KEY_FLOAT);
        if ((value == null) || "".equals(value))
        {
            result.add(new ValidationResult(
                IDemo5Constants.MEMENTO_KEY_FLOAT,
                MyGuidedConfigOperationStatus.ERROR_EMPTY_FLOAT()));
        }

        // check the boolean value for all tree rows
        if (rootMemento.isMemento(IDemo5Constants.MEMENTO_KEY_TREE))
        {
            for (Memento child : rootMemento.getMemento(IDemo5Constants.MEMENTO_KEY_TREE).getChildren())
            {
                validateTree(child, result);
            }
        }
        return result;
    }

    /**
     * Own function and not part of the Backend class. It validates the tree rows and a warning is added, if the Boolean
     * cell has value false.
     *
     * @param treeMemento
     * @param result
     */
    private void validateTree(Memento treeMemento, ArrayList<ValidationResult> result)
    {
        for (Memento child : treeMemento.getChildren())
        {
            validateTree(child, result);
        }
        if ((treeMemento.getString(IDemo5Constants.MEMENTO_COLUMN_BOOL) == null)
            || treeMemento.getString(IDemo5Constants.MEMENTO_COLUMN_BOOL).equals("false"))
        {
            result.add(new ValidationResult(
                treeMemento.getPath() + "/" + IDemo5Constants.MEMENTO_COLUMN_BOOL,
                MyGuidedConfigOperationStatus.WARNING_INVALID_BOOLEAN_TREE_CELL()));

        }
    }

    /**
     * This method is called asynchronously after {@link #doValidate()} each time the wizard configuration did change to
     * update the enablement of the widgets.<br>
     * <br>
     * Disables the checkbox widget.
     */
    @Override
    public void doUpdateWidgetEnablement()
    {
        // permanently disable the checkbox
        changeWidgetEnablement(IDemo5Constants.WIDGET_ID_CHECKBOX, false);
    }

    /**
     * This method is called directly after validation has finished.
     * <p>
     * It is used to disable the Next button, if there are validation errors. If no errors are reported, the Next button
     * is enabled again.
     * <p>
     * Furthermore this method can be used to change the background and foreground colors of table and treetable cells,
     * e.g.
     *
     * <pre>
     * rowMemento.setString(SWTTableController.ATTRIBUTE_PREFIX_BGCOLOR + IConstants.MEMENTOID_COLUMN_INT, &quot;#FF8040&quot;);
     * </pre>
     *
     * Note that memento attributes are changed in order to do this. Thus {@link #updateView(List)} has to be called
     * with the list of the changed memento paths, so the changes are adopted in the GUI.
     * <p>
     */
    @Override
    public void doValidationFinished(List<ValidationResult> results)
    {
        // if an error occured, then the next button is disabled
        for (ValidationResult result : results)
        {
            if (result.getStatus().getSeverity() == APIOperationStatus.SEVERITY_ERROR)
            {
                getPage().setNextButtonEnable(false);
                return;
            }
        }
        getPage().setNextButtonEnable(true);
    }

    /**
     * This method is called when the user presses the finish button and contains the actual wizard code.<br>
     * <br>
     * Does not change the project model. Retrieves the validation results using {@link #getValidationResults()} and
     * returns the result messages.
     *
     * @return List of errors or null
     */
    @Override
    public List<APIOperationStatus> doRun()
    {
        List<ValidationResult> resultList = getValidationResults();
        ArrayList<APIOperationStatus> result = new ArrayList<APIOperationStatus>();
        if (resultList.isEmpty())
        {
            return result;
        }
        for (ValidationResult res : resultList)
        {
            result.add(res.getStatus());
        }
        return result;
    }

}
