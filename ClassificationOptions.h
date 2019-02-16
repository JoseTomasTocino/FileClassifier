#ifndef CLASSIFICATIONOPTIONS_H
#define CLASSIFICATIONOPTIONS_H

#include <QString>

enum class ClassificationCriteria {
    None,
    ByYear,
    ByYearAndMonth,
    ByYearMonthAndDay
};

struct ClassificationOptions {
    QString mSourceFolder;
    QString mOutputFolder;
    ClassificationCriteria mClassificationCriteria = ClassificationCriteria::ByYearAndMonth;

    bool mUseCopy = false;
    bool mUseExtensionFolders = false;
    bool mRecursive = false;
    bool mOnlyMedia = false;
};

#endif // CLASSIFICATIONOPTIONS_H
