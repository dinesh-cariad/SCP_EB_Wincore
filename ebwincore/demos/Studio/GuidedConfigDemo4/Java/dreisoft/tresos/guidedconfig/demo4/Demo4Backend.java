/*
 * Copyright (C) Elektrobit Automotive GmbH
 * All rights reserved
 */

package dreisoft.tresos.guidedconfig.demo4;

import dreisoft.tresos.guidedconfig.api.backend.AbstractBackend;
import dreisoft.tresos.guidedconfig.api.backend.ValidationResult;
import dreisoft.tresos.guidedconfig.api.gui.form.SWTTreeTable;
import dreisoft.tresos.guidedconfig.api.gui.form.SWTTreeTableController;
import dreisoft.tresos.guidedconfig.api.memento.Memento;
import dreisoft.tresos.guidedconfig.demo4.generated.operationstatus.MyGuidedConfigOperationStatus;

import java.util.ArrayList;
import java.util.List;

/**
 * This class is the controller of a wizard. It validates the widgets. While running the wizard the results are logged
 * to the ErrorLog.
 *
 * <p>
 * <i> Copyright (C) Elektrobit Automotive GmbH - All rights reserved</i>
 * </p>
 *
 * @since 2009.a
 */
public class Demo4Backend extends AbstractBackend
{

    @Override
    public List<ValidationResult> doValidate()
    {
        ArrayList<ValidationResult> result = new ArrayList<ValidationResult>();
        Memento rootMemento = getMemento();

        // add an error if the Integer field is empty
        String value = rootMemento.getString(IDemo4Constants.MEMENTO_KEY_TEXT);
        if ((value == null) || "".equals(value))
        {
            result.add(new ValidationResult(
                IDemo4Constants.MEMENTO_KEY_TEXT,
                MyGuidedConfigOperationStatus.ERROR_EMPTY_INTEGER()));
        }

        // add an error if the Float field is empty
        value = rootMemento.getString(IDemo4Constants.MEMENTO_KEY_FLOAT);
        if ((value == null) || "".equals(value))
        {
            result.add(new ValidationResult(
                IDemo4Constants.MEMENTO_KEY_FLOAT,
                MyGuidedConfigOperationStatus.ERROR_EMPTY_FLOAT()));
        }

        // validate each row of the table
        // add an error if the Integer field is empty
        // add a warning if the Boolean cell is empty
        Memento tableMemento = rootMemento.getMemento(IDemo4Constants.MEMENTO_KEY_TABLE);
        if (tableMemento != null)
        {
            int noChilds = tableMemento.getNumberOfChildren();
            for (int i = 0; (i < noChilds) && (i < 50000); i++)
            {
                if (isValidationCanceled())
                {
                    return result;
                }
                Memento m = tableMemento.getMemento(i);
                if (m == null)
                {
                    break;
                }

                if ((m.getString(IDemo4Constants.MEMENTO_COLUMN_INT) == null)
                    || "".equals(m.getString(IDemo4Constants.MEMENTO_COLUMN_INT)))
                {
                    result.add(new ValidationResult(
                        IDemo4Constants.MEMENTO_KEY_TABLE + "/" + i + "/" + IDemo4Constants.MEMENTO_COLUMN_INT,
                        MyGuidedConfigOperationStatus.ERROR_EMPTY_INTEGER_CELL(i)));
                }
                if ((m.getString(IDemo4Constants.MEMENTO_COLUMN_BOOL) == null)
                    || m.getString(IDemo4Constants.MEMENTO_COLUMN_BOOL).equals("false"))
                {
                    result.add(new ValidationResult(
                        IDemo4Constants.MEMENTO_KEY_TABLE + "/" + i + "/" + IDemo4Constants.MEMENTO_COLUMN_BOOL,
                        MyGuidedConfigOperationStatus.WARNING_EMPTY_BOOLEAN_CELL(i)));
                }
            }
        }
        // check the boolean value for all tree rows
        if (rootMemento.isMemento(IDemo4Constants.MEMENTO_KEY_TREE))
        {
            for (Memento child : rootMemento.getMemento(IDemo4Constants.MEMENTO_KEY_TREE).getChildren())
            {
                if (isValidationCanceled())
                {
                    return result;
                }
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
        SWTTreeTable tree = (SWTTreeTable)((SWTTreeTableController)getWidgetFactory().getControllersByWidgetId(IDemo4Constants.WIDGET_ID_TREE)
                                                                                     .get(0)).doGetWidget();
        if (tree.isReadOnly(treeMemento))
        {
            return;
        }
        if ((treeMemento.getString(IDemo4Constants.MEMENTO_COLUMN_BOOL) == null)
            || treeMemento.getString(IDemo4Constants.MEMENTO_COLUMN_BOOL).equals("false"))
        {
            result.add(new ValidationResult(
                treeMemento.getPath() + "/" + IDemo4Constants.MEMENTO_COLUMN_BOOL,
                MyGuidedConfigOperationStatus.WARNING_INVALID_BOOLEAN_TREE_CELL()));

        }
    }

    @Override
    public void doUpdateWidgetEnablement()
    {
        // disable the checkbox
        changeWidgetEnablement(IDemo4Constants.WIDGET_ID_CHECKBOX, false);
    }
}
