package test.tresos.plugin.generator.cds;

import dreisoft.tresos.datamodel2.api.model.DCtxt;
import dreisoft.tresos.generator.ng.api.ant.IJavaGenTemplate;
import dreisoft.tresos.generator.ng.api.ant.JavaGenContext;
import dreisoft.tresos.generator.ng.api.cds.CDSGenFile;
import dreisoft.tresos.generator.ng.api.cds.abstraction.CompilerAbstraction;
import dreisoft.tresos.generator.ng.api.cds.abstraction.MemAbstraction;
import dreisoft.tresos.generator.ng.api.cds.backend.CBackend;
import dreisoft.tresos.generator.ng.api.cds.ctypes.Array;
import dreisoft.tresos.generator.ng.api.cds.ctypes.Ptr;
import dreisoft.tresos.generator.ng.api.cds.ctypes.UInt16;
import dreisoft.tresos.generator.ng.api.cds.ctypes.UInt32;

import java.io.IOException;

/**
 * The CDS demo generator. By implementing interface {@link IJavaGenTemplate} it is possible to run this generator as
 * Java generator in an Ant build script using task <code>ng.javagen</code> respective <code>ng.javagen.src</code>.
 *
 * <p>
 * <i> (C) Elektrobit Automotive GmbH - All rights reserved</i>
 * </p>
 *
 * @since 2008.b
 */
public class CDSDemoGenerator implements IJavaGenTemplate
{

    /**
     * The generate method.
     *
     * @see dreisoft.tresos.generator.ng.api.ant.IJavaGenTemplate#generate(dreisoft.tresos.generator.ng.api.ant.JavaGenContext)
     * @since 2008.b
     */
    @Override
    public void generate(JavaGenContext context)
    {
        // With the JavaGenContext you can access the DCtxt to query the module configuration
        DCtxt ctxt = context.gen.getDCtxtVariable();

        // start a CDSGenFile with default writer from context
        CDSGenFile cdsFile;
        try
        {
            // use a CBackend instance to generate C code
            cdsFile = context.cds.startFile(new CBackend());
        }
        catch (IOException e)
        {
            e.printStackTrace();
            return;
        }

        // start default scope
        cdsFile.startScope();

        // write typedef of struct Point
        cdsFile.type(new Point());

        // write typedef of struct Rectangle
        cdsFile.type(new Rectangle());

        // end last scope and start a new one with different scope attributes
        cdsFile.endScope();
        cdsFile.startScope(new MemAbstraction("EEP_START_SEC_VAR_16BIT", "EEP_STOP_SEC_VAR_16BIT"));

        // read a configuration value
        long exampleInt = ctxt.var.getInt("ExamplesCommon/Int");
        // write a simple UInt16
        cdsFile.var(new UInt16("exampleInt", exampleInt));
        // end scope to write the memory abstraction end
        cdsFile.endScope();

        cdsFile.startScope(new MemAbstraction("EEP_START_SEC_VAR_32BIT", "EEP_STOP_SEC_VAR_32BIT"));
        // write some UInt32 with explicit comments
        cdsFile.var(new UInt32("abc", 34557).setComment("Explicit comment for variable abc."));
        // and with compiler abstraction
        cdsFile.var(new UInt32("def", 999999).setCompilerAbstraction(new CompilerAbstraction(true))
                                             .setComment("constant UInt32 def"));
        // end scope to write the memory abstraction end block
        cdsFile.endScope();

        // start default scope
        cdsFile.startScope();

        // write a Rectangle (do not assign a names to the given Points!)
        Rectangle rect1 = new Rectangle("rect1", new Point(11, 7), new Point(125, 80));
        cdsFile.var(rect1);

        // write a Pointer to a Point int Rectanlge rect1 depending on configuration parameter FeatureCan
        Ptr<Point> pPoint;
        boolean hasCAN = ctxt.var.getBool("ExamplesResourceFile/FeatureCan");
        if (hasCAN)
        {
            pPoint = new Ptr<Point>("p2Point", rect1.a);
        }
        else
        {
            pPoint = new Ptr<Point>("p2Point", rect1.b);
            pPoint.setComment("The configuration doesn't feature CAN.");
        }
        cdsFile.var(pPoint);

        // generate a typedef of Number
        cdsFile.type(new Number());

        // write an array of Numbers
        Array<Number> numbers = new Array<Number>("numbers");
        numbers.setComment("You can initialize arrays in different ways.");
        // note that Array is an enclosing type, so you must not set names to the elements
        numbers.add(new Number(2)).add(new Number(4)).add(new Number(8)).add(new Number(16));
        cdsFile.var(numbers);

        cdsFile.endScope();

        // close cdsFile
        cdsFile.close();
    }
} // CDSDemoGenerator

