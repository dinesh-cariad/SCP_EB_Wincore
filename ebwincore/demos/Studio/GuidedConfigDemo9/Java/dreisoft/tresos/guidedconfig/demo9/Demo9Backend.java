/*
 * Copyright (C) EB Automotive
 * All rights reserved
 */

package dreisoft.tresos.guidedconfig.demo9;

import dreisoft.tresos.guidedconfig.api.backend.AbstractBackend;
import dreisoft.tresos.guidedconfig.api.backend.ValidationResult;
import dreisoft.tresos.guidedconfig.api.gui.databinding.MementoOperationHandler;
import dreisoft.tresos.guidedconfig.api.memento.AbstractMementoFactory;
import dreisoft.tresos.guidedconfig.api.memento.Memento;
import dreisoft.tresos.guidedconfig.demo9.generated.operationstatus.MyGuidedConfigOperationStatus;

import java.util.ArrayList;
import java.util.List;

/**
 * This demo shows how to place guided configuration widgets into another view/dialog outside of a guided config page.
 */
public class Demo9Backend extends AbstractBackend
{

    /**
     * Validates the memento structure for the table widget
     */
    @Override
    public List<ValidationResult> doValidate()
    {
        ArrayList<ValidationResult> result = new ArrayList<ValidationResult>();
        Memento rootMemento = getMemento();

        // validate the values of the table visible in the PropertiesView and the external dialog
        Memento tableMemento = rootMemento.getMemento(IConstants.MEMENTO_KEY_TABLE);
        if (tableMemento != null)
        {
            int noChilds = tableMemento.getNumberOfChildren();
            for (int i = 0; (i < noChilds) && (i < 50000); i++)
            {
                Memento rowMemento = tableMemento.getMemento(i);
                if (rowMemento != null)
                {
                    // if the String value is empty, then an error is shown
                    if ((rowMemento.getString(IConstants.MEMENTO_COLUMN_STRING) == null)
                        || "".equals(rowMemento.getString(IConstants.MEMENTO_COLUMN_STRING)))
                    {
                        result.add(new ValidationResult(
                            IConstants.MEMENTO_KEY_TABLE + "/" + i + "/" + IConstants.MEMENTO_COLUMN_STRING,
                            MyGuidedConfigOperationStatus.DRAFT_ERROR(IConstants.MEMENTO_COLUMN_STRING
                                + " must not be empty.")));
                    }
                    // if the checkbox is unchecked, a warning is shown
                    if ((rowMemento.getString(IConstants.MEMENTO_COLUMN_BOOL) == null)
                        || rowMemento.getString(IConstants.MEMENTO_COLUMN_BOOL).equals("false"))
                    {
                        result.add(new ValidationResult(
                            rowMemento.getPath() + "/" + IConstants.MEMENTO_COLUMN_BOOL,
                            MyGuidedConfigOperationStatus.DRAFT_WARNING("treeCell 'Boolean' is false!")));
                    }
                    // if the integer value is lower than 5 than an error is shown
                    if ((rowMemento.getString(IConstants.MEMENTO_COLUMN_INT) == null)
                        || (rowMemento.getInteger(IConstants.MEMENTO_COLUMN_INT) < 5))
                    {
                        result.add(new ValidationResult(
                            rowMemento.getPath() + "/" + IConstants.MEMENTO_COLUMN_INT,
                            MyGuidedConfigOperationStatus.DRAFT_ERROR("treeCell 'Integer' lower 5!")));
                    }
                }
            }
        }
        return result;
    }

    /**
     * This method removes all entries of the table in the properties view
     *
     * @since 11
     */
    private void emptyRows()
    {
        // get the root memento
        Memento rootMemento = getMemento();
        // retrieve the table memento
        Memento tableMemento = rootMemento.getMemento(IConstants.MEMENTO_KEY_TABLE);
        MementoOperationHandler moh = getMementoOperationHandler();

        // get all memento paths of the table children
        ArrayList<String> childPaths = new ArrayList<String>();
        for (Memento child : moh.getRows(tableMemento))
        {
            childPaths.add(child.getPath());
        }
        // unset all mementos of the table children
        moh.unsetMementos(childPaths.toArray(new String[0]));
    }

    /**
     * Called when in the GUI data was changed after the data was written to the mementos and after the GUI was so far
     * updated for the change.
     *
     * In this example the tree will be checked if the last tree entry has been deleted, to update the
     * TreePropertiesView.
     */
    @Override
    public void doHandleUpdatedData(List<String> affectedPaths)
    {
        // if the last tree entry has been deleted, then also the PropertiesView shall not show values anymore.
        // check if the last tree entry was deleted
        boolean treeAffected = false;
        for (String path : affectedPaths)
        {
            if (path.startsWith(getMemento().getMemento(IConstants.MEMENTO_KEY_TREE).getPath()))
            {
                treeAffected = true;
                break;
            }
        }
        // ...and remove all table entries in the properties view
        if (treeAffected)
        {
            Memento tree = getMemento().getMemento(IConstants.MEMENTO_KEY_TREE);
            if (tree.getChildren().size() == 0)
            {
                emptyRows();
                updateView(IConstants.MEMENTO_KEY_TREE);
            }
        }
    }

    @Override
    public AbstractMementoFactory doGetMementoFactory()
    {
        return Demo9MementoFactory.INSTANCE;
    }
}
