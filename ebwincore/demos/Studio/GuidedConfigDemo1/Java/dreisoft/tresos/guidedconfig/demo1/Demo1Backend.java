/*
 * Copyright (C) Elektrobit Automotive GmbH
 * All rights reserved
 */

package dreisoft.tresos.guidedconfig.demo1;

import dreisoft.tresos.guidedconfig.api.backend.AbstractBackend;
import dreisoft.tresos.guidedconfig.api.backend.ValidationResult;
import dreisoft.tresos.guidedconfig.api.memento.AbstractMementoFactory;
import dreisoft.tresos.guidedconfig.api.memento.Memento;
import dreisoft.tresos.guidedconfig.demo1.generated.operationstatus.MyGuidedConfigOperationStatus;

import java.util.ArrayList;
import java.util.List;

/**
 * Backend class for Demo1 which has its own MementoFactory. Only part of the mementos are persistent. The backend
 * validates if the integer field of page2 has a value.
 *
 * <p>
 * <i> Copyright (C) Elektrobit Automotive GmbH - All rights reserved</i>
 * </p>
 *
 * @since 2009.a
 */
public class Demo1Backend extends AbstractBackend
{

    @Override
    public List<ValidationResult> doValidate()
    {
        // this method does the validation of the current memento data

        // Initialize the result list which will contain the validation errors
        ArrayList<ValidationResult> result = new ArrayList<ValidationResult>();

        // Get rootMemento of wizard
        Memento rootMemento = getMemento();

        // querying the field as string enables you to check for emptyness/nonexistence
        String value = rootMemento.getString(Demo1MementoFactory.MEMENTOID_PAGE2_INTEGER);
        // show an error if the field is empty
        if ((value == null) || "".equals(value))
        {
            result.add(new ValidationResult(
                Demo1MementoFactory.MEMENTOID_PAGE2_INTEGER,
                MyGuidedConfigOperationStatus.ERROR_PAGE2_EMPTY_INTEGER()));
        }
        return result;
    }

    @Override
    public AbstractMementoFactory doGetMementoFactory()
    {
        // overwrite the default implementation so that we are able to return our custom memento factory
        return Demo1MementoFactory.INSTANCE;
    }

} // MyDemo1Backend

