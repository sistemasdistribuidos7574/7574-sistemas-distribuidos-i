package ar.uba.fi.contabilidapp.upload;

import com.google.common.collect.ImmutableList;
import org.junit.Test;

import static ar.uba.fi.contabilidapp.upload.InputFileTestData.INVALID_LINES;
import static ar.uba.fi.contabilidapp.upload.InputFileTestData.VALID_LINES;
import static org.assertj.core.api.Assertions.assertThat;

public class LineValidatorTest {

    @Test
    public void isValidTest_validLines_returnsTrue() {
        for (String validLine : VALID_LINES) {
            assertThat(LineValidator.isValid(validLine)).isTrue();
        }
    }

    @Test
    public void isValidTest_invalidLines_returnsFalse() {
        for (String validLine : INVALID_LINES) {
            assertThat(LineValidator.isValid(validLine)).isFalse();
        }
    }

}
